/** Write a program that opens a file (with the open() system call) and then
calls fork() to create a new process. Can both the child and parent access the file
descriptor returned by open()? What happens when they are writing to the file
concurrently, i.e., at the same time? **/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include <fcntl.h>

int main() {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        const char *child_msg = "Child process writing to the file\n";
        write(fd, child_msg, 34);
    } else {
        // Parent process
        const char *parent_msg = "Parent process writing to the file\n";
        // wait(NULL);
        write(fd, parent_msg, 35);
        sleep(1);
    }

    close(fd);

    return 0;
}

