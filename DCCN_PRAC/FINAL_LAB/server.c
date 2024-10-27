#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/wait.h>

#define UDP_PORT 4444
#define TCP_PORT 9999
#define INTERFACE "eth0"  // Specify the network interface

// Signal handler to clean up child processes to avoid zombies
void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Function to retrieve the IP address of a specified interface
void get_interface_ip(const char *interface, char *ip_buffer, size_t ip_buffer_size) {
    int sockfd;
    struct ifreq ifr;

    // Create a socket to perform the ioctl call
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        strncpy(ip_buffer, "Error", ip_buffer_size);
        return;
    }

    // Get the IP address of the specified interface
    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0'; // Null terminate
    if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0) {
        perror("ioctl error");
        strncpy(ip_buffer, "Error", ip_buffer_size);
        close(sockfd);
        return;
    }

    // Copy the IP address into the buffer
    struct sockaddr_in *ip_addr = (struct sockaddr_in *)&ifr.ifr_addr;
    inet_ntop(AF_INET, &ip_addr->sin_addr, ip_buffer, ip_buffer_size);
    close(sockfd);
}

// Handle client TCP requests in a child process
void  handle_client(int client_sock) {
    char buffer[1024];
    char *responses[] = {
        "Police station number: 100",
        "Ambulance number: 101",
        "Fire station number: 102",
        "Vehicle repair number: 103",
        "Food delivery: 104",
        "Blood bank number: 105"
    };

    // Receive request from the client
    int n = recv(client_sock, buffer, sizeof(buffer), 0);
    if (n < 0) {
        perror("Error reading from TCP socket");
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    int choice = atoi(buffer) - 1;

    // Send the corresponding response or an error if invalid choice
    if (choice >= 0 && choice < 6) {
        if (send(client_sock, responses[choice], strlen(responses[choice]), 0) < 0) {
            perror("Error writing to TCP socket");
        }
    } else {
        char *invalid = "Invalid choice";
        if (send(client_sock, invalid, strlen(invalid), 0) < 0) {
            perror("Error writing invalid message to TCP socket");
        }
    }

    close(client_sock);
    exit(EXIT_SUCCESS);
}

// UDP server to respond with the server's IP address
void udp_server() {
    int udp_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];
    char server_ip[INET_ADDRSTRLEN];

    // Create UDP socket
    if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind UDP socket to the address and port
    if (bind(udp_sock, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("UDP bind failed");
        close(udp_sock);
        exit(EXIT_FAILURE);
    }

    printf("UDP: Waiting for client request...\n");

    // Continuously listen for incoming UDP requests
    while (1) {
        int n = recvfrom(udp_sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            perror("UDP receive error");
            continue; // Continue to listen to other clients
        }

        buffer[n] = '\0';
        printf("UDP: Received client message: %s\n", buffer);

        // Get the server's IP address from eth0
        get_interface_ip(INTERFACE, server_ip, sizeof(server_ip));

        // Send the server IP address back to the client
        if (sendto(udp_sock, server_ip, strlen(server_ip), 0, (struct sockaddr *)&client_addr, client_len) < 0) {
            perror("UDP: Error sending IP address");
        } else {
            printf("UDP: Sent IP address %s to client\n", server_ip);
        }
    }

    close(udp_sock);
}

// TCP server to handle multiple clients using fork
void tcp_server() {
    int tcp_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pid_t child_pid;

    // Create TCP socket
    if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TCP_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind TCP socket
    if (bind(tcp_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("TCP bind failed");
        close(tcp_sock);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming TCP connections
    if (listen(tcp_sock, 5) < 0) {
        perror("TCP listen failed");
        close(tcp_sock);
        exit(EXIT_FAILURE);
    }

    printf("TCP: Server listening on port %d...\n", TCP_PORT);

    // Set up signal handler to clean up zombie processes
    struct sigaction sa;
    sa.sa_handler = handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("Error setting up SIGCHLD handler");
        exit(EXIT_FAILURE);
    }

    // Main loop to accept and handle incoming connections
    while (1) {
        client_sock = accept(tcp_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("TCP accept failed");
            continue;  // Continue accepting other clients
        }

        printf("TCP: Client connected\n");

        // Fork a new process to handle the client
        child_pid = fork();
        if (child_pid == 0) {
            // Child process handles the client
            close(tcp_sock);  // Close listening socket in child
            handle_client(client_sock);
        } else if (child_pid > 0) {
            // Parent process
            close(client_sock);  // Close client socket in parent
        } else {
            perror("Fork failed");
            close(client_sock);
        }
    }

    close(tcp_sock);
}

int main() {
    // Start the UDP server in a separate process
    pid_t pid = fork();
    if (pid == 0) {
        udp_server();  // Child process handles UDP server
    } else if (pid > 0) {
        tcp_server();  // Parent process handles TCP server
    } else {
        perror("Failed to fork UDP server");
        exit(EXIT_FAILURE);
    }

    return 0;
}
