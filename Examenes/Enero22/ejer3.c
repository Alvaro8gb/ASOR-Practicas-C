 // Escribe un programa que cree dos procesos y ejecute un comando en cada uno, de forma que la salida del primer comando 
 // sea la entrada del segundo. El proceso padre imprimirá el PID y el código de salida de cada proceso. 
 // El programa se ejecutará de 
 // la siguiente manera: $ ./conecta comando1 argumento1 comando2 argumento2 Por ejemplo: $ ./conecta ls -l wc -c   

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main(int argc, char ** argv){

   if (argc != 5) {
        fprintf(stderr, "Usage: %s comando1 argumento1 comando2 argumento2 \nPor ejemplo: $ ./conecta ls -l wc -c \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    pid_t pid_1, pid_2;

    int tub = pipe(pipefd );

    pid_1 = fork();

    if (pid_1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid_1 == 0) {   

        close(pipefd[1]);          /* Close unused write end */

        dup2(pipefd[0], STDIN_FILENO);  /*Redirect Stantard Input to read end*/

        close(pipefd[0]);

        execlp(argv[3],argv[3], argv[4], NULL);

    } else {  // Padre

        int status;

        pid_2 = fork();
        
        if (pid_2 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid_2 == 0) { 

            close(pipefd[0]);          /* Close unused red end */

            dup2(pipefd[1], STDOUT_FILENO);  /*Redirect Stantard Output to write end*/

            close(pipefd[1]);

            execlp(argv[1], argv[1], argv[2], NULL);


        }else{ // Padre again

            close(pipefd[0]);
            close(pipefd[1]);

            waitpid(pid_1, &status, 0);

            printf(" Hijo 1 : %d status: %d\n",pid_1, status);

            waitpid(pid_2, &status, 0);

            printf(" Hijo 2 : %d status: %d\n",pid_2, status);

        }
    }      

    return 0;
 }