/** Write a program that creates a child process, and then in the child closes standard
output (STDOUT FILENO). What happens if the child calls printf() to print
some output after closing the descriptor? **/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>


int main() {

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {  
        close(STDOUT_FILENO);
        printf("This will not be printed to the terminal.\n");
    } else {
        // close(STDOUT_FILENO);
        printf("Parent process: This will be printed to the terminal.\n");
    }

    return 0;
}