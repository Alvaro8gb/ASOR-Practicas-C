
#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(){

    char buf;

    int p_h[2], h_p[2];
    
    if ( pipe(p_h) == -1) handle_error("Error in pipe()");
    if ( pipe(h_p) == -1) handle_error("Error in pipe()");

    pid_t pid = fork();

    switch (pid) {
        case -1:
            handle_error("Error in fork()");
        case 0: //Hijo 

            close(p_h[1]);         
            close(h_p[0]);
            close(STDIN_FILENO);

            for( int i= 0; i<3 ; i++){

                while (read(p_h[0], &buf, 1) > 0 && buf!='\0' ){
                    write(STDOUT_FILENO, &buf, 1);
                }

                write(STDOUT_FILENO,"\n------\n" , 9);

                sleep(1);

                write(h_p[1], "l", 1);
            }

            write(h_p[1],"q",1);
            close(h_p[1]);
            close(p_h[0]);

            break;
        default: //Padre 

            close(p_h[0]);         
            close(h_p[1]);
            close(STDOUT_FILENO);

            do{

                while (read(STDIN_FILENO, &buf, 1) > 0 && buf!='\n'){
                    write(p_h[1], &buf, 1);
                }
                write(p_h[1], "\0", 1); // Avisar al padre fin de mensaje
                

            }while(read(h_p[0], &buf,1) > 0 && buf == 'l');

            
            close(h_p[0]);
            close(p_h[1]);   
            
            break;
    }

    return 0;

}