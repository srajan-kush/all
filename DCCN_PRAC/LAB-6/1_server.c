#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_file(FILE *fp, int sockfd) {
    char data[BUFFER_SIZE] = {0};

    while (fgets(data, BUFFER_SIZE, fp) != NULL) {
        if (send(sockfd, data, sizeof(data), 0) == -1) {
            perror("[-] Error in sending file.");
            exit(1);
        }
        memset(data, 0, BUFFER_SIZE);
    }
    send(sockfd, "EOF", 4, 0); // Send EOF marker when the file is done
}

int main() {
    char buffer[BUFFER_SIZE];
    char filename[100];
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }
    printf("[+] Server listening on port %d\n", PORT);

    // Accepting incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }



    
    // Receiving the command (RETR filename)
    recv(new_socket, buffer, BUFFER_SIZE, 0);
    if (strncmp(buffer, "RETR", 4) == 0) {
        sscanf(buffer, "RETR %s", filename);  // Extracting the filename
        printf("[+] Client requested file: %s\n", filename);

        // Open the file and send it
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            perror("[-] File not found.");
            send(new_socket, "File not found", 15, 0);
        } else {
            send_file(fp, new_socket);
            fclose(fp);
            printf("[+] File %s sent successfully.\n", filename);
        }
    } else {
        printf("[-] Invalid command\n");
        send(new_socket, "Invalid command", 16, 0);
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
