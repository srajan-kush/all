#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

#define PORT 8080
#define BACKLOG 100

void log_message(const char *filename, const char *message) {
    FILE *log_file = fopen(filename, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    // Get current time
    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';  // Remove newline character from time

    fprintf(log_file, "[%s] %s\n", time_str, message);
    fclose(log_file);
}

int main() {

    int sock_fd, client_fd;
    struct sockaddr_in myaddr, client_addr;
    socklen_t addrlen = sizeof(client_addr);  // Correct length for client address

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Creating socket failed\n");
        exit(1);
    }

    // binding
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(PORT);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    memset(myaddr.sin_zero, '\0', sizeof(myaddr.sin_zero));

    if (bind(sock_fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("Binding failed\n");
        close(sock_fd);
        exit(1);
    }

    if (listen(sock_fd, BACKLOG) < 0) {
        perror("Error in Backlog\n"); 
        close(sock_fd);
        exit(1);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        if ((client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &addrlen)) < 0) {
            perror("Acceptance failed\n");
            exit(1);
        }

        char log_msg[1024];
        sprintf(log_msg, "Server got connection from: %s", inet_ntoa(client_addr.sin_addr));
        log_message("server_log.txt", log_msg);

        printf("%s\n", log_msg);

        if (fork() == 0) {  // Child process
            close(sock_fd);  // Close the listening socket in the child process

            char buffer[1024];
            while (1) {
                memset(buffer, 0, sizeof(buffer));

                // Read data from client
                int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
                if (bytes_read <= 0) {
                    perror("Receiving error or connection closed\n");
                    break;
                }

                sprintf(log_msg, "Received from %s: %s", inet_ntoa(client_addr.sin_addr), buffer);
                log_message("server_log.txt", log_msg);

                // Send data back to client (echo)
                if (send(client_fd, buffer, strlen(buffer), 0) <= 0) {
                    perror("Sending error\n");
                    break;
                }

                sprintf(log_msg, "Sent to %s: %s", inet_ntoa(client_addr.sin_addr), buffer);
                log_message("server_log.txt", log_msg);
            }

            close(client_fd);
            exit(0);  // Terminate the child process
        }

        close(client_fd);  // Parent process closes the connected client socket
    }

    return 0;
}
