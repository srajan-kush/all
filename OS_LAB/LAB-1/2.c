#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed!\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process:\n");
        int ChildVar = 55;
        printf("Assign values to this variable ChildVar: %d\n", ChildVar);
    } else {
        // Parent process
        printf("Parent process:\n");
        int ParentVar = 110;
        printf("Assign values to this variable ParentVar: %d\n", ParentVar);
    }

    return 0;
}
