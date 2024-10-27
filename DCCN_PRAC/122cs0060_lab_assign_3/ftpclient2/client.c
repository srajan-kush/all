#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);  // Change to SOCK_DGRAM for UDP
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("UDP FTP Client: Enter command (MKDIR, SAVE, RETR, LIST): ");
        bzero(command, BUFFER_SIZE);
        fgets(command, BUFFER_SIZE, stdin);
        command[strlen(command) - 1] = '\0';  // Remove newline

        // Send the command to the server
        sendto(client_socket, command, strlen(command), 0, (struct sockaddr *)&server_addr, addr_len);

        // Parse command to see if it's SAVE, so we can send file content
        char cmd[10], arg[50];
        sscanf(command, "%s %s", cmd, arg);

        if (strncmp(cmd, "SAVE", 4) == 0) {
            printf("Enter the content to save: ");
            bzero(buffer, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE, stdin);
            sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len);
        }

        // Receive response from the server
        bzero(buffer, BUFFER_SIZE);
        recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
        printf("%s\n", buffer);
    }

    close(client_socket);
    return 0;
}
