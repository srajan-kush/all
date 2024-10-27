#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT    8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE]; // Sending client messages
    char buffer2[MAXLINE]; // Receiving server messages
    struct sockaddr_in servaddr;

    // UDP socket creation
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    
    // Assign Server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int n, len;
    int size_arr;
    while(1) {
        printf("Enter size of array: ");
        scanf("%d", &size_arr);

        printf("Enter numbers (space-separated): ");
        for (int i = 0; i < size_arr; i++) {
            int num;
            scanf("%d", &num);
            buffer[i] = (char)num;
        }

        // Send the buffer of numbers
        sendto(sockfd, buffer, size_arr,
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,
            sizeof(servaddr));

        // Receive the server response
        n = recvfrom(sockfd, (char *)buffer2, MAXLINE,
                MSG_WAITALL, (struct sockaddr *) &servaddr,
                &len);
        buffer2[n] = '\0';
        printf("Server : %s\n", buffer2);

        // Clear the buffers
        memset(buffer, 0, MAXLINE);
        memset(buffer2, 0, MAXLINE);
    }

    close(sockfd);
    return 0;
}
