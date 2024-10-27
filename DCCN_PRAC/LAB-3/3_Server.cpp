#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX 1024
#define PORT 15001
#define SA struct sockaddr

// Helper function to check if a string is a palindrome
bool isPalindrome(char* str)
{
    int len = strlen(str);
    int start = 0;
    int end = len - 1;

    while (start < end)
    {
        if (str[start] != str[end])
        {
            return false;
        }
        start++;
        end--;
    }

    return true;
}

void HandleRequest(int sockfd)
{
    char buff[MAX];
    int n;

    for(;;){
        // Read the list of strings from the client
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));

        // Split the input into strings using the '|' delimiter
        int palindromeCount = 0;
        char* token = strtok(buff, "|");

        printf("Strings from client:\n");
        while (token != NULL)
        {
            printf("%s\n",token);
            // Check if the current string is a palindrome
            if (isPalindrome(token))
            {
                palindromeCount++;
            }

            // Get the next string
            token = strtok(NULL, "|");
        }

        // Send the palindrome count back to the client
        bzero(buff, sizeof(buff));
        sprintf(buff, "%d", palindromeCount);
        write(sockfd, buff, sizeof(buff));
    }
}

int main()
{
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli;

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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding the socket to the given IP and PORT
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("Socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    len = sizeof(cli);

    // Accept the data packet from client
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0)
    {
        printf("Server accept failed...\n");
        exit(0);
    }
    else
        printf("Server accepted the client...\n");

    // Handle client requests
    HandleRequest(connfd);

    // Close the connection
    close(connfd);
    close(sockfd);
}
