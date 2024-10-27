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
#define MAX_CLIENTS 5
#define MAX_FILE_SIZE (1L * 1024 * 1024 * 1024) // 1 GB limit

int active_clients = 0;

void handle_client(int client_socket, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE], argument[BUFFER_SIZE];

    while (1) {
        bzero(buffer, BUFFER_SIZE);
        read(client_socket, buffer, BUFFER_SIZE);

        if (strlen(buffer) == 0) continue;

        sscanf(buffer, "%s %s", command, argument);

        printf("Client %s:%d: request received %s %s\n", 
            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), command, argument);

        if (strncmp(command, "MKDIR", 5) == 0) {
            if (mkdir(argument, 0777) == 0)
                write(client_socket, "Directory created", 17);
            else
                write(client_socket, "Failed to create directory", 25);
        }
        else if (strncmp(command, "SAVE", 4) == 0) {
            int file_fd = open(argument, O_WRONLY | O_CREAT, 0666);
            if (file_fd < 0) {
                write(client_socket, "Failed to save file", 19);
            } else {
                bzero(buffer, BUFFER_SIZE);
                read(client_socket, buffer, BUFFER_SIZE);
                write(file_fd, buffer, strlen(buffer));
                close(file_fd);
                write(client_socket, "File saved", 10);
            }
        }
        else if (strncmp(command, "RETR", 4) == 0) {
            int file_fd = open(argument, O_RDONLY);
            if (file_fd < 0) {
                write(client_socket, "File not found", 14);
            } else {
                struct stat file_stat;
                fstat(file_fd, &file_stat);
                if (file_stat.st_size > MAX_FILE_SIZE) {
                    write(client_socket, "File size exceeds limit (1 GB)", 30);
                } else {
                    // Send file in chunks
                    while (1) {
                        bzero(buffer, BUFFER_SIZE);
                        int bytes_read = read(file_fd, buffer, BUFFER_SIZE);
                        if (bytes_read == 0) break; // End of file
                        write(client_socket, buffer, bytes_read);
                    }
                    printf("File %s sent to the client\n", argument);
                }
                close(file_fd);
            }
        }
        else if (strncmp(command, "LIST", 4) == 0) {
            struct dirent *dir;
            DIR *d = opendir(argument);
            if (d) {
                while ((dir = readdir(d)) != NULL) {
                    write(client_socket, dir->d_name, strlen(dir->d_name));
                    write(client_socket, "\n", 1);
                }
                closedir(d);
            } else {
                write(client_socket, "Directory not found", 19);
            }
        } else {
            write(client_socket, "Invalid command", 15);
        }
    }
    close(client_socket);
    active_clients--;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
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

    listen(server_socket, MAX_CLIENTS);
    printf("FTP Server: Ready to serve clients commands\n");

    while (1) {
        if (active_clients >= MAX_CLIENTS) {
            printf("Maximum client limit reached\n");
            // Optionally delay further accept attempts to avoid busy loop
            sleep(1);
            continue;
        }

        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        active_clients++;

        if (fork() == 0) {  // Child process to handle the client
            close(server_socket);
            handle_client(client_socket, client_addr);
            exit(0);
        } else {  // Parent process continues to accept other clients
            close(client_socket);
        }
    }

    close(server_socket);
    return 0;
}
