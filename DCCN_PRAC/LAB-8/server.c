#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void *handle_client(void *arg);

typedef struct {
    int socket;
    int client_id;
} client_info;

client_info clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void encode_message(char *message, char key) {
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] ^= key;  // Simple XOR encoding
    }
}

void *handle_client(void *arg) {
    client_info *client = (client_info *)arg;
    char buffer[BUFFER_SIZE];
    char key = 'K';  // Example key for encoding
    int bytes_read;

    while ((bytes_read = recv(client->socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_read] = '\0';

        // If the client wants to exit
        if (strcmp(buffer, "exit") == 0) {
            printf("Client %d disconnected.\n", client->client_id);
            break;
        }

        // Encode the message
        encode_message(buffer, key);
        printf("Received from Client %d: %s\n", client->client_id, buffer);

        // Broadcast the encoded message to all clients
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < client_count; i++) {
            // Send the encoded message to all clients except the sender
            if (clients[i].socket != client->socket) {
                send(clients[i].socket, buffer, strlen(buffer), 0);
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    // Close the socket and remove the client
    close(client->socket);
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == client->socket) {
            clients[i] = clients[--client_count];  // Remove client
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    free(client);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t tid;

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    // Accept clients in a loop
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_mutex_lock(&mutex);
        if (client_count < MAX_CLIENTS) {
            clients[client_count].socket = new_socket;
            clients[client_count].client_id = client_count + 1;
            printf("New client connected: %d\n", clients[client_count].client_id);
            pthread_create(&tid, NULL, handle_client, (void *)&clients[client_count]);
            client_count++;
        } else {
            printf("Maximum clients reached. Connection refused.\n");
            close(new_socket);
        }
        pthread_mutex_unlock(&mutex);
    }

    close(server_fd);
    return 0;
}
