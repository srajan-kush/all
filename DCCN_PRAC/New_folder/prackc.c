#include<stdio.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define PORT 8080
#define ipaddress "127.0.0.1"

int main(){
    char *buffer="hello ";
    read(0,buffer, strlen(buffer));
    printf("%s", buffer);

    return 0;
}