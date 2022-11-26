#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    umask(0027);

    int fd = open("example_file", O_CREAT | O_WRONLY | O_TRUNC , 00645);
    
    if( fd == -1){
        printf("Error creando fichero");
        exit(EXIT_FAILURE); 
    }
    
    close(fd);

    return 0;
}