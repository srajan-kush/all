#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8085
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(1);
    }

    while (1) {
        printf("FTP Client: Enter command (MKDIR, SAVE, RETR, LIST): ");
        bzero(command, BUFFER_SIZE);
        fgets(command, BUFFER_SIZE, stdin);
        command[strlen(command) - 1] = '\0';  // Remove newline

        write(client_socket, command, strlen(command));

        char cmd[10], arg[50];
        sscanf(command, "%s %s", cmd, arg);

        if (strncmp(cmd, "SAVE", 4) == 0) {
            printf("Enter the content to save: ");
            bzero(buffer, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE, stdin);
            write(client_socket, buffer, strlen(buffer));
        }
        else if (strncmp(cmd, "RETR", 4) == 0) {
            // Receive file size first
            char file_size_str[64];
            bzero(file_size_str, sizeof(file_size_str));
            read(client_socket, file_size_str, sizeof(file_size_str));
            long file_size = atol(file_size_str);

            // Receive file and save it in the client's home directory
            char home_directory[256];
            snprintf(home_directory, sizeof(home_directory), "%s/%s", getenv("HOME"), arg);

            int file_fd = open(home_directory, O_WRONLY | O_CREAT, 0666);
            if (file_fd < 0) {
                perror("Failed to open file for writing");
            } else {
                long bytes_received = 0;
                while (bytes_received < file_size) {
                    bzero(buffer, BUFFER_SIZE);
                    int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
                    if (bytes_read <= 0) break; // End of file
                    write(file_fd, buffer, bytes_read);
                    bytes_received += bytes_read;
                }
                printf("File %s saved to home directory\n", arg);
                close(file_fd);
            }
        }


        bzero(buffer, BUFFER_SIZE);
        read(client_socket, buffer, BUFFER_SIZE);
        printf("%s\n", buffer);
    }

    close(client_socket);
    return 0;
}
