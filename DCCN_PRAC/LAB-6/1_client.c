#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pwd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to save the file in the home directory
void save_file(int sockfd, char *filename) {
    char buffer[BUFFER_SIZE];
    char filepath[256];
    
    // Get home directory
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    snprintf(filepath, sizeof(filepath), "%s/%s", homedir, filename);

    FILE *fp = fopen(filepath, "w");
    if (fp == NULL) {
        perror("[-] Error in opening file.");
        exit(1);
    }

    while (1) {
        int n = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (strncmp(buffer, "EOF", 3) == 0) {
            printf("[+] File received successfully.\n");
            break;
        }
        fprintf(fp, "%s", buffer);
        memset(buffer, 0, BUFFER_SIZE);
    }
    fclose(fp);
    printf("[+] File saved at: %s\n", filepath);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char filename[100];
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Input filename to retrieve
    printf("Enter the filename to retrieve: ");
    scanf("%s", filename);

    // Send RETR command to server
    snprintf(buffer, sizeof(buffer), "RETR %s", filename);
    send(sock, buffer, strlen(buffer), 0);

    // Receive and save the file
    save_file(sock, filename);

    close(sock);
    return 0;
}
