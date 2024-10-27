#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pp[2];
    char buffer[100];
    pipe(pp);
    
    pid_t pid = fork();

    if (pid < 0) {
        printf("Error Occurs\n");
        return 1;
    } else if (pid == 0) {
        printf("AIDS\n");
        read(pp[0],buffer,100);
        printf("%s",buffer);
    } else {
        write(pp[1],"CSE\n",4);
    }

    return 0;
}
