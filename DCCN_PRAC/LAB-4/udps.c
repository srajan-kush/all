#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT     8080
#define MAXLINE  1024
#define CLADDR_LEN 100

// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    char buffer2[MAXLINE];
    char clientAddr[CLADDR_LEN];
    struct sockaddr_in servaddr, cliaddr;
    
    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    
    // Assign Server address
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    // Bind the UDP socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    int len, n;
    len = sizeof(cliaddr); 

    while(1) {
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }

        inet_ntop(AF_INET, &(cliaddr.sin_addr), clientAddr, CLADDR_LEN); // Convert client IP address to string format
        printf("Client %s\n", clientAddr);
        buffer[n] = '\0';

        // Check if numbers are in ascending order
        int is_ascending = 1;
        for (int i = 1; i < n; i++) {
            if (buffer[i] < buffer[i - 1]) {
                is_ascending = 0;
                break;
            }
        }

        // Sort the array (Bubble Sort)
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (buffer[j] > buffer[j + 1]) {
                    char temp = buffer[j];
                    buffer[j] = buffer[j + 1];
                    buffer[j + 1] = temp;
                }
            }
        }

        // Prepare the response
        if (is_ascending) {
            snprintf(buffer2, MAXLINE, "Numbers are in ascending order\nMaximum number: %d\nSecond minimum: %d\n", 
                    (int)buffer[n - 1], (int)buffer[1]);
        } else {
            snprintf(buffer2, MAXLINE, "Numbers are not in ascending order\nMaximum number: %d\nSecond minimum: %d\n", 
                    (int)buffer[n - 1], (int)buffer[1]);
        }

        // Send the response back to the client
        sendto(sockfd, buffer2, strlen(buffer2), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

        // Clear buffers
        memset(buffer, 0, MAXLINE);
        memset(buffer2, 0, MAXLINE);
    }

    close(sockfd);
    return 0;
}
