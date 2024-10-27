#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char longest_string[BUFFER_SIZE] = "";

    while (1) {
        // Clear buffer for each new set of data
        memset(buffer, 0, BUFFER_SIZE);

        // Read data from the client
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("Client disconnected.\n");
            } else {
                perror("Failed to read from client");
            }
            close(client_socket);
            break;
        }

        printf("Received data: %s\n", buffer);

        // Split the received data into individual strings using space as a separator
        char *token = strtok(buffer, " ");
        while (token != NULL) {
            if (strlen(token) > strlen(longest_string)) {
                strcpy(longest_string, token);
            }
            token = strtok(NULL, " ");
        }

        printf("Longest string found: %s\n", longest_string);

        // Send the longest string back to the client
        if (write(client_socket, longest_string, strlen(longest_string) + 1) < 0) {  // +1 to include the null terminator
            perror("Failed to send longest string");
            close(client_socket);
            break;
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        printf("Waiting for a connection...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        printf("Connection accepted...\n");

        handle_client(new_socket);
    }

    return 0;
}
