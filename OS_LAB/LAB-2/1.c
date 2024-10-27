#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int x = 10;
    pid_t pid = fork();

    if (pid < 0) {
        printf("Error Occurs\n");
        return 1;
    } else if (pid == 0) {
        printf("Child process: Initial value of x = %d\n", x);
        x = 20;
        printf("Child process: Updated value of x = %d\n", x);
    } else {
        printf("Parent process: Initial value of x = %d\n", x);
        x = 30;
        printf("Parent process: Updated value of x = %d\n", x);
    }

    return 0;
}
