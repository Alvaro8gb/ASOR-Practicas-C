
#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define USAGE "Usage: %s command1 argument1 command2 argument2\nExample: %s echo 12345 wc -c\n"
#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(){

    char buf;

    int p_h[2], h_p[2];
    
    if ( pipe(p_h) == -1) handle_error("Error in pipe()");
    if ( pipe(h_p) == -1) handle_error("Error in pipe()");

    //pipefd[0] -> extremo de escritura
    //pipefd[1] -> extremo de lectura

    pid_t pid = fork();

    switch (pid) {
        case -1:
            handle_error("Error in fork()");
        case 0: //Hijo 

            close(p_h[1]);         
            close(h_p[0]);
            close(STDIN_FILENO);

            while (read(p_h[0], &buf, 1) > 0)
                write(STDOUT_FILENO, &buf, 1);

            close(p_h[0]);

            sleep(1);

            write(h_p[1],"1",1);

            char msg[7];

            for( int i= 0; i<10 ; i++){
                sprintf(msg, "msg %d",i);
                write(h_p[1], msg, 7);

            }

            write(h_p[1],"q",1);

            close(h_p[1]);


            break;
        default: //Padre 

            close(p_h[0]);         
            close(h_p[1]);
            close(STDOUT_FILENO);

            while (read(STDIN_FILENO, &buf, 1) > 0 && buf!='\n'){
                write(p_h[1], &buf, 1);
            }

            close(p_h[1]);   

            while(buf != '1' ){
                read(h_p[0],&buf,1);
            }      

            close(h_p[0]);
     

            

            break;
    }


    return 0;

}