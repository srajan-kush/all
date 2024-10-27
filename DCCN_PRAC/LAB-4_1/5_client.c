#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    while (1) {
        char all_strings[BUFFER_SIZE] = "";  // Buffer to hold the concatenated sentence
        char temp[BUFFER_SIZE];  // Temporary buffer to hold each sentence input

        printf("Enter a sentence (or type 'exit' to quit): ");
        fgets(temp, BUFFER_SIZE, stdin);
        temp[strcspn(temp, "\n")] = '\0';  // Remove trailing newline

        // Check if the user wants to exit
        if (strcmp(temp, "exit") == 0) {
            break;
        }

        // Copy the sentence into the all_strings buffer
        strcpy(all_strings, temp);

        // Send the sentence to the server
        if (write(sock, all_strings, strlen(all_strings) + 1) < 0) {  // +1 to include the null terminator
            perror("Failed to send sentence");
            return -1;
        }

        // Receive the longest string from the server
        if (read(sock, buffer, BUFFER_SIZE) < 0) {
            perror("Failed to receive data from server");
            return -1;
        }
        printf("Longest word from server: %s\n", buffer);
    }

    // Close the socket
    close(sock);

    return 0;
}
