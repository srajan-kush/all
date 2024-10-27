#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX 1024
#define PORT 15001
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX];
    char input[MAX];
    int n;

    for(;;){
        // Sending the list of strings
        printf("Enter a list of strings separated by '|': ");
        bzero(input, sizeof(input));
        n = 0;
        while ((input[n++] = getchar()) != '\n');

        write(sockfd, input, sizeof(input));

        // Read palindrome count from the server
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server: Number of palindromes = %s\n", buff);
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    // Socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    // Assign IP and PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to the server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("Connection with the server failed...\n");
        exit(0);
    }
    else
        printf("Connected to the server..\n");

    // Function to communicate with the server
    func(sockfd);

    // Close the socket
    close(sockfd);
}
