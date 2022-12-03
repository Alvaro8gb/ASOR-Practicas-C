
#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


#define USAGE "Usage: %s command arguments\n"
void redirect(int fd_old, char path[], int flags){

    int fd = open(path, flags, 0600);

    if ( fd == -1 ){
        fprintf(stderr, "Error in open() : %s\n", path);
        exit(EXIT_FAILURE);
    }
    
    if ( dup2(fd, fd_old) == -1){
        close(fd);
        perror("Error en dup()\n");
        exit(EXIT_FAILURE);
    }

}

int main(int argc, char **argv){

    if ( argc == 1){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }  

    pid_t pid = fork();

    switch (pid) {
        case -1:
            perror("Error in fork()");
            exit(EXIT_FAILURE);
        case 0: //Hijo
            waitpid(getppid(), NULL, 0);
            printf("Hijo %i (padre: %i)\n", getpid(), getppid());
            break;
        
        default:
            printf("Padre %i (hijo: %i)\n", getpid(), pid);
            exit(EXIT_SUCCESS);
    }

    // Demon set up
    
    if ( chdir("/tmp") == -1){
            perror("Error in chdir()");
            exit(EXIT_FAILURE);
    }

    if ( setsid() == -1){
        perror("Error in setsid()");
        exit(EXIT_FAILURE);
    }

    redirect(0, "/dev/null",  O_RDONLY );
    redirect(1, "/tmp/daemon.out", O_CREAT | O_WRONLY | O_TRUNC);
    redirect(2, "/tmp/daemon.err",O_CREAT | O_WRONLY | O_TRUNC);

    // Service logic
    
    if ( execvp(argv[1], &argv[1]) == -1){
        perror("Error in execvp()");
        exit(EXIT_FAILURE);
    }

    return 0;

}