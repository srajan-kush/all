#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    // Creating a child process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        fprintf(stderr, "Fork failed!\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process: \n");
        printf("PID of child process: %d\n", getpid());
        printf("PID of parent process (from child): %d\n", getppid());
    } else {
        // Parent process
        printf("Parent process: \n");
        printf("PID of parent process: %d\n", getpid());
        printf("PID of child process (from parent): %d\n", pid);
    }

    return 0;
}
