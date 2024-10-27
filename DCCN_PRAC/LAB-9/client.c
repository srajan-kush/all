#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>   
#include <sys/time.h> 

#define PORT 8078
#define BUFFER_SIZE 1024
#define ENCODE_KEY 9

void decode_message(char* message, int key) {
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] ^= key;
    }
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    int key = ENCODE_KEY;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    fd_set fds;
    int max_fd;

    while (1) {
        FD_ZERO(&fds);
        FD_SET(sock, &fds);
        FD_SET(STDIN_FILENO, &fds);
        max_fd = sock;

        if (select(max_fd + 1, &fds, NULL, NULL, NULL) > 0) {
            if (FD_ISSET(STDIN_FILENO, &fds)) {
                // Read input from user
                fgets(message, BUFFER_SIZE, stdin);
                message[strcspn(message, "\n")] = 0; // Remove newline

                send(sock, message, strlen(message), 0);
                if (strcmp(message, "exit") == 0) {
                    printf("Exiting...\n");
                    break;
                }
            }

            if (FD_ISSET(sock, &fds)) {
                // Receive message from server
                int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
                if (bytes_received > 0) {
                    buffer[bytes_received] = '\0';
                    decode_message(buffer, key);
                    printf("Decoded message: %s\n", buffer);
                }
            }
        }
    }

    close(sock);
    return 0;
}
