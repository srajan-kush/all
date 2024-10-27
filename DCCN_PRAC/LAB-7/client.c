#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 80

int main()
{
    int sock;
    struct sockaddr_in6 server_addr;
    char buffer[BUFFER_SIZE];
    int bytes_sent, bytes_received;

    // Create the socket
    sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket() failed");
        exit(-1);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET6, "::1", &server_addr.sin6_addr) <= 0) {
        perror("inet_pton() failed");
        close(sock);
        exit(-1);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect() failed");
        close(sock);
        exit(-1);
    }

    // Send data to the server
    strcpy(buffer, "Hello from client");
    bytes_sent = send(sock, buffer, strlen(buffer), 0);
    if (bytes_sent < 0) {
        perror("send() failed");
        close(sock);
        exit(-1);
    }

    printf("Sent %d bytes to server: %s\n", bytes_sent, buffer);

    // Receive data from the server
    bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("recv() failed");
        close(sock);
        exit(-1);
    }

    buffer[bytes_received] = '\0';
    printf("Received from server: %s\n", buffer);

    // Close the socket
    close(sock);

    return 0;
}
