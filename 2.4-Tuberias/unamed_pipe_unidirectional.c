
#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define USAGE "Usage: %s command1 argument1 command2 argument2\nExample: %s echo 12345 wc -c\n"
#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char ** argv){

    if ( argc != 5){
        fprintf(stderr,USAGE, argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if ( pipe(pipefd) == -1) handle_error("Error in pipe()");

    //pipefd[0] -> extremo de lectura
    //pipefd[1] -> extremo de escritura

    pid_t pid = fork();

    switch (pid) {
        case -1:
            handle_error("Error in fork()");
        case 0: //Hijo lee de la tuberia

            close(pipefd[1]);          /* Close unused write end */

            dup2(pipefd[0], STDIN_FILENO);  /*Redirect Stantard Input to read end*/

            close(pipefd[0]);

            execlp(argv[3],argv[3], argv[4], NULL);
            
            break;
        default: //Padre escribe en la tuberia

            close(pipefd[0]);          /* Close unused read end */

            dup2(pipefd[1], STDOUT_FILENO);  /*Redirect Stantard Output to write end*/

            close(pipefd[1]);        

            execlp(argv[1], argv[1], argv[2], NULL);

            break;
    }


    return 0;

}