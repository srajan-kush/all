#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

int count_lines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int lines = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }

    fclose(file);
    return lines;
}

int count_words(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file");
        return -1;
    }

    int words = 0;
    char ch;
    int in_word = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (isspace(ch)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            words++;
        }
    }

    fclose(file);
    return words;
}

int main() {
    const char *filename = "test.txt";
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed!\n");
        return 1;
    } else if (pid == 0) {

        int word_count = count_words(filename);
        if (word_count >= 0) {
            printf("Child process: Number of words = %d\n", word_count);
        }
    } else {
       
        int line_count = count_lines(filename);
        if (line_count >= 0) {
            printf("Parent process: Number of lines = %d\n", line_count);
        }
        wait(NULL); 
    }

    return 0;
}
