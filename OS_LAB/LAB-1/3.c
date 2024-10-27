#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int fact(int n){
    if(n == 1) return 1;
    return n * fact(n - 1);
}

int main(){
    pid_t pid;

    int n;
    printf("Enter a number :- ");
    scanf("%d",&n);

    pid = fork();

    if(pid < 0){
        printf("Process Failed !!\n");
        return 1;
    }else if(pid > 0){
        printf("Parent Process:\n");
        printf("Factorial is : %d\n",fact(n));
    }else{
        printf("Child Process:\n");
        printf("Factorial is : %d\n",fact(n));
    }

    return 0;
}