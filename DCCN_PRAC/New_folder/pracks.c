#include<stdio.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define PORT 8080
#define BACKLOG 100

int main(){

    int sock_fd,new_fd;
    struct sockaddr_in myaddr,new_addr;
    int myaddr_len = sizeof(myaddr);

    if((sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        perror("socket\n");
        exit(EXIT_FAILURE);
    }

    // binding

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(PORT);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    memset(myaddr.sin_zero,'\0',sizeof(myaddr));

    if(bind(sock_fd,(struct sockaddr *)&myaddr,sizeof(myaddr)) < 0){
        perror("binding\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // listning
    if(listen(sock_fd,BACKLOG) < 0){
        perror("Backlog");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listning on port %d\n",PORT);


    while(1){
        if((new_fd = accept(sock_fd,(struct sockaddr *)&new_addr,(socklen_t *)&myaddr_len)) < 0){
            perror("accepting");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }
        printf("Acceping message from %s\n",inet_ntoa(new_addr.sin_addr));

        char result[1024] = {0};
        if(read(new_fd,result,sizeof(result)) < 0){
            perror("reading problem\n");
            close(sock_fd);
            close(new_fd);
            exit(EXIT_FAILURE);
        }
        printf("%s",result);
        close(new_fd);
    }
    close(sock_fd);



    return 0;
}