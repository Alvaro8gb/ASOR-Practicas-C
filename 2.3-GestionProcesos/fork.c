#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    pid_t pid = fork();

    switch (pid) {
        case -1:
            perror("Error in fork()");
            exit(EXIT_FAILURE);
        case 0:
            printf("Hijo %i (padre: %i)\n", getpid(), getppid());
            break;
        default:
            printf("Padre %i (hijo: %i)\n", getpid(), pid);
            break;
    }

}