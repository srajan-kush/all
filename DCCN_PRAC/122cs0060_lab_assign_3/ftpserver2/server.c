#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int server_socket, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE], argument[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    while (1) {
        bzero(buffer, BUFFER_SIZE);
        recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);

        if (strlen(buffer) == 0) continue;

        sscanf(buffer, "%s %s", command, argument);
        
        printf("Client %s:%d: request received %s %s\n", 
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), command, argument);

        if (strncmp(command, "MKDIR", 5) == 0) {
            if (mkdir(argument, 0777) == 0)
                sendto(server_socket, "Directory created", 17, 0, (struct sockaddr *)&client_addr, addr_len);
            else
                sendto(server_socket, "Failed to create directory", 25, 0, (struct sockaddr *)&client_addr, addr_len);
        }
        else if (strncmp(command, "SAVE", 4) == 0) {
            int file_fd = open(argument, O_WRONLY | O_CREAT, 0666);
            if (file_fd < 0) {
                sendto(server_socket, "Failed to save file", 19, 0, (struct sockaddr *)&client_addr, addr_len);
            } else {
                bzero(buffer, BUFFER_SIZE);
                recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
                write(file_fd, buffer, strlen(buffer));
                close(file_fd);
                sendto(server_socket, "File saved", 10, 0, (struct sockaddr *)&client_addr, addr_len);
            }
        }
        else if (strncmp(command, "RETR", 4) == 0) {
            int file_fd = open(argument, O_RDONLY);
            if (file_fd < 0) {
                sendto(server_socket, "File not found", 14, 0, (struct sockaddr *)&client_addr, addr_len);
            } else {
                bzero(buffer, BUFFER_SIZE);
                read(file_fd, buffer, BUFFER_SIZE);
                sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addr_len);
                close(file_fd);
            }
        }
        else if (strncmp(command, "LIST", 4) == 0) {
            struct dirent *dir;
            DIR *d = opendir(argument);
            if (d) {
                while ((dir = readdir(d)) != NULL) {
                    sendto(server_socket, dir->d_name, strlen(dir->d_name), 0, (struct sockaddr *)&client_addr, addr_len);
                    sendto(server_socket, "\n", 1, 0, (struct sockaddr *)&client_addr, addr_len);
                }
                closedir(d);
            } else {
                sendto(server_socket, "Directory not found", 19, 0, (struct sockaddr *)&client_addr, addr_len);
            }
        } else {
            sendto(server_socket, "Invalid command", 15, 0, (struct sockaddr *)&client_addr, addr_len);
        }
    }
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);  // Change to SOCK_DGRAM for UDP
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    printf("UDP FTP Server: Ready to serve clients commands\n");

    handle_client(server_socket, client_addr);

    close(server_socket);
    return 0;
}
