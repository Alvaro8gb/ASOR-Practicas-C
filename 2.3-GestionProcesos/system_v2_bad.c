#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define USAGE "Usage: %s command -arguments\n"

int main(int argc, char ** argv){

    if ( argc == 1){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }  

    if ( execvp(argv[1], &argv[1]) == -1 ){
        perror("Error in system()");
        exit(EXIT_FAILURE);
    }

    printf("Command executed\n"); // No se muestra este mensaje

    return 0;
}