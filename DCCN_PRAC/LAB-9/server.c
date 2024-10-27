#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8078
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define ENCODE_KEY 9

int clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void encode_message(char* message, int key) {
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] ^= key;
    }
}

void decode_message(char* message, int key) {
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] -= key;
    }
}

void broadcast_message(char* message, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != 0 && clients[i] != sender_sock) {
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void* handle_client(void* client_socket) {
    int sock = *(int*)client_socket;
    char buffer[BUFFER_SIZE];
    int key = ENCODE_KEY;

    while (1) {
        int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            close(sock);
            pthread_mutex_lock(&clients_mutex);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i] == sock) {
                    clients[i] = 0;
                    break;
                }
            }
            pthread_mutex_unlock(&clients_mutex);
            printf("Client disconnected\n");
            break;
        }

        buffer[bytes_received] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            printf("Client requested exit\n");
            close(sock);
            pthread_mutex_lock(&clients_mutex);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i] == sock) {
                    clients[i] = 0;
                    break;
                }
            }
            pthread_mutex_unlock(&clients_mutex);
            break;
        }

        // Encode the message
        encode_message(buffer, key);

        printf("Encoded message: %s\n", buffer);

        // Send encoded message to all clients
        broadcast_message(buffer, sock);
    }

    free(client_socket);
    return NULL;
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    pthread_t tid;

    while (1) {
        new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (new_socket < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] == 0) {
                clients[i] = new_socket;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        int* new_sock = malloc(sizeof(int));
        *new_sock = new_socket;
        pthread_create(&tid, NULL, handle_client, (void*)new_sock);
    }

    return 0;
}
