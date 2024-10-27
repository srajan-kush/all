#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#define PORT 2121
#define BUFFER_SIZE 1024

void list_files(int sock);
void send_file(int sock, char *filename);
void receive_file(int sock, char *filename);
void make_directory(int sock, char *dirname);

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char command[5], argument[BUFFER_SIZE];

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("172.16.25.121");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to FTP server\n");

    while (1) {
        printf("ftp> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        sscanf(buffer, "%s %s", command, argument);

        if (strcmp(command, "LIST") == 0) {
            send(sock, buffer, strlen(buffer), 0);
            list_files(sock);
        } else if (strcmp(command, "RETR") == 0) {
            send(sock, buffer, strlen(buffer), 0);
            receive_file(sock, argument);
        } else if (strcmp(command, "STOR") == 0) {
            send(sock, buffer, strlen(buffer), 0);
            send_file(sock, argument);
        } else if (strcmp(command, "MKD") == 0) {
            send(sock, buffer, strlen(buffer), 0);
            make_directory(sock, argument);
        } else if (strcmp(command, "EXIT") == 0) {
            break;
        } else {
            printf("Invalid command\n");
        }
    }

    close(sock);
    return 0;
}

void list_files(int sock) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("%s", buffer);
    }
}

void send_file(int sock, char *filename) {
    char buffer[BUFFER_SIZE];
    int file = open(filename, O_RDONLY);
    if (file < 0) {
        perror("File not found");
        return;
    }

    int bytes_read;
    while ((bytes_read = read(file, buffer, BUFFER_SIZE)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }
    close(file);
}

void receive_file(int sock, char *filename) {
    char buffer[BUFFER_SIZE];
    int file = open(filename, O_WRONLY | O_CREAT, 0666);
    if (file < 0) {
        perror("File creation failed");
        return;
    }

    int bytes_received;
    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        write(file, buffer, bytes_received);
    }
    close(file);
}

void make_directory(int sock, char *dirname) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("%s", buffer);
    }
}
