//Escribe un programa que lea simultáneamente de una tubería con nombre,
// que existirá previamente en el directorio actual de trabajo y se llamará tuberia, y del terminal.
// ● Imprimirá en el terminal los datos leídos y terminará tras 5 segundos sin recibir datos (1
// punto).
// ● Al detectar el fin de fichero en la tubería, la cerrará y volverá a abrirla (

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int MAX_BUF = 200;


void read_input(int fd){
    char buff[MAX_BUF];
    int num_read = 0;

    do{
        num_read = read(fd, buff, MAX_BUF);

        if( num_read == -1) handle_error("Error in read()");
          
        if ( write(STDOUT_FILENO, buff, num_read) == -1) handle_error("Error in write()");
        
    }while(num_read == MAX_BUF);

}
int main(void){

    fd_set rfds;
    struct timeval tv;
    int retval;

    char tuberia[] = "tuberia_nombre";

    int fd = open(tuberia, O_RDONLY | O_NONBLOCK, 00600);

    /* Watch stdin (fd 0) to see when it has input. */

    do {

        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        FD_SET(fd, &rfds);

        tv.tv_sec = 5;
        tv.tv_usec = 0;

        retval = select(fd + 1, &rfds, NULL, NULL, &tv);
        /* Don't rely on the value of tv now! */

        if (retval == -1) handle_error("erro slect()");

        if ( FD_ISSET(fd, &rfds)){
            printf("Reding in tub \n");

            read_input(fd);

            close(fd);

            fd = open(tuberia, O_RDONLY | O_NONBLOCK, 00600);
        
        }

        if ( FD_ISSET(STDIN_FILENO, &rfds)){
            printf("Reding in terminal \n");

            read_input(STDIN_FILENO);
        }


    }while(retval != 0);


    return 0;


}