#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PORT_NU 3000
#define DEST_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define FILENAME "sample.txt"

int main() {
    FILE *file;
    char buffer[BUFFER_SIZE * 10];  // Buffer to hold all lines from the file
    char reversed_buffer[BUFFER_SIZE * 10]; // Buffer to receive reversed text
    char info_buffer[BUFFER_SIZE];  // Buffer to receive the line/word count info
    int clientFd;

    // Open sample.txt for reading
    file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Create UDP socket
    if ((clientFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    struct sockaddr_in serverAddr;
    socklen_t serverAddrLen = sizeof(serverAddr);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT_NU);

    if (inet_pton(AF_INET, DEST_IP, &serverAddr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(clientFd);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Read all lines from the file into the buffer
    memset(buffer, 0, sizeof(buffer));
    char line[BUFFER_SIZE];
    while (fgets(line, BUFFER_SIZE, file) != NULL) {
        strcat(buffer, line);  // Append each line to the buffer
    }

    // Send all lines to the server
    sendto(clientFd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr*)&serverAddr, serverAddrLen);

    // Receive the reversed lines from the server
    int n = recvfrom(clientFd, reversed_buffer, sizeof(reversed_buffer), MSG_WAITALL, (struct sockaddr*)&serverAddr, &serverAddrLen);
    reversed_buffer[n] = '\0';  // Null-terminate the received data
    printf("Reversed lines from server:\n%s\n", reversed_buffer);

    // Receive the info (line count, word count) from the server
    n = recvfrom(clientFd, info_buffer, sizeof(info_buffer), MSG_WAITALL, (struct sockaddr*)&serverAddr, &serverAddrLen);
    info_buffer[n] = '\0';  // Null-terminate the received data
    printf("Info from server: %s\n", info_buffer);

    // Close the file and socket
    fclose(file);
    close(clientFd);

    return 0;
}
