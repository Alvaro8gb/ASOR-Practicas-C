#include <stdlib.h> // Macros erres
#include <stdio.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/file.h>

int main(){

    int fd = open("fichero", O_WRONLY | O_CREAT);

    if (fd == -1){
        perror("Error en open()");
        exit(EXIT_FAILURE);
    }  

    if( lockf(fd, F_TLOCK, 40) == -1){
        close(fd); 
        perror("Error bloqueando fichero");
        exit(EXIT_FAILURE);
    }

    time_t t = time(NULL);

    if ( t == -1){
        close(fd); 
        perror("Error en time()");
        exit(EXIT_FAILURE); 
    }

    printf("%s\n", ctime(&t));
    sleep(10);

    lockf(fd, F_ULOCK, 5);
    sleep(10);
    close(fd); 

    return 0;
}