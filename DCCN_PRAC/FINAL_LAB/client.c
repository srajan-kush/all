#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define UDP_PORT 4444
#define TCP_PORT 9999
#define SERVER_UDP_ADDR "127.0.0.1"  // Address for contacting the server's UDP port

void show_menu() {
    printf("Select the service you need:\n");
    printf("1. Police station number\n");
    printf("2. Ambulance number\n");
    printf("3. Fire station number\n");
    printf("4. Vehicle repair number\n");
    printf("5. Food delivery\n");
    printf("6. Blood bank number\n");
    printf("Enter your choice: ");
}

int main() {
    int udp_sock, tcp_sock;
    struct sockaddr_in udp_server_addr, tcp_server_addr;
    char buffer[1024];
    socklen_t addr_len = sizeof(udp_server_addr);
    char server_ip[INET_ADDRSTRLEN];

    // UDP: Create socket for requesting server's IP
    if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&udp_server_addr, 0, sizeof(udp_server_addr));
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_port = htons(UDP_PORT);
    udp_server_addr.sin_addr.s_addr = inet_addr(SERVER_UDP_ADDR);

    // Send a request for server's IP
    strcpy(buffer, "Get IP");
    if (sendto(udp_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&udp_server_addr, addr_len) < 0) {
        perror("UDP: Error sending request");
        close(udp_sock);
        exit(EXIT_FAILURE);
    }

    printf("UDP: Request sent for server IP\n");

    // Receive the server's IP address
    int n = recvfrom(udp_sock, server_ip, sizeof(server_ip), 0, (struct sockaddr *)&udp_server_addr, &addr_len);
    if (n < 0) {
        perror("UDP: Error receiving server IP");
        close(udp_sock);
        exit(EXIT_FAILURE);
    }
    server_ip[n] = '\0';
    printf("UDP: Server IP Address received: %s\n", server_ip);

    close(udp_sock);  // Close UDP socket

    // TCP: Create socket for connecting to the server
    if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&tcp_server_addr, 0, sizeof(tcp_server_addr));
    tcp_server_addr.sin_family = AF_INET;
    tcp_server_addr.sin_port = htons(TCP_PORT);
    tcp_server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // Connect to the server using TCP
    if (connect(tcp_sock, (struct sockaddr *)&tcp_server_addr, sizeof(tcp_server_addr)) < 0) {
        perror("TCP connection failed");
        close(tcp_sock);
        exit(EXIT_FAILURE);
    }

    printf("TCP: Connected to server\n");

    // Show menu and get user input
    show_menu();
    int choice;
    scanf("%d", &choice);
    snprintf(buffer, sizeof(buffer), "%d", choice);

    // Send request to the server
    if (send(tcp_sock, buffer, strlen(buffer), 0) < 0) {
        perror("TCP: Error sending request");
        close(tcp_sock);
        exit(EXIT_FAILURE);
    }

    printf("TCP: Request sent for service %d\n", choice);

    // Receive the response from the server
    n = recv(tcp_sock, buffer, sizeof(buffer), 0);
    if (n < 0) {
        perror("TCP: Error receiving response");
        close(tcp_sock);
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("TCP: Server response: %s\n", buffer);

    close(tcp_sock);
    return 0;
}
