#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PORT_NU 3000
#define BUFFER_SIZE 1024

// Helper function to reverse a string
void reverse(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

// Helper function to count words in a string
int count_words(char *str) {
    int count = 0;
    char *token = strtok(str, " \n");
    while (token != NULL) {
        count++;
        token = strtok(NULL, " \n");
    }
    return count;
}

int main() {
    int socketFd;
    char buffer[BUFFER_SIZE * 10]; // Buffer to hold all the lines sent by the client

    // Create UDP socket
    if ((socketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT_NU);

    // Bind the socket
    if (bind(socketFd, (const struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(socketFd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT_NU);

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Receive all lines from the client
        int n = recvfrom(socketFd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (n < 0) {
            perror("Receive failed");
            continue;
        }
        buffer[n] = '\0';  // Null-terminate the received data

        printf("Received all lines from client:\n%s\n", buffer);

        int line_count = 0, total_words = 0;
        char reversed_buffer[BUFFER_SIZE * 10] = "";

        int i=0, j=0;
        char ch=buffer[i];
        char temp[BUFFER_SIZE];

        while(ch!='\0'){
            if(ch=='\n')temp[j++]='\0';
            else temp[j++]=ch;

            if(ch=='\n'){
                line_count++;
                total_words++;

                reverse(temp);
                strcat(reversed_buffer, temp);
                strcat(reversed_buffer, "\n");

                memset(temp, 0, BUFFER_SIZE);
                j=0;
            }
            if(ch==' '){
                total_words++;
            }
            i++;
            ch=buffer[i];
            
        }

        
        // Send the reversed lines back to the client
        sendto(socketFd, reversed_buffer, strlen(reversed_buffer), MSG_CONFIRM, (const struct sockaddr*)&clientAddr, clientAddrLen);

        // Prepare and send info about the number of lines and words
        char info[BUFFER_SIZE];
        snprintf(info, sizeof(info), "Total Lines: %d, Total Words: %d", line_count, total_words);
        sendto(socketFd, info, strlen(info), MSG_CONFIRM, (const struct sockaddr*)&clientAddr, clientAddrLen);

        printf("Processed and sent data back to client.\n");
    }

    close(socketFd);
    return 0;
}
