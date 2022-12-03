#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define USAGE "Usage: %s 'command arguments'\n"

int main(int argc, char ** argv){

    if ( argc != 2){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }  

    if ( system(argv[1]) == -1 ){
        perror("Error in system()");
        exit(EXIT_FAILURE);
    }

    printf("Command executed\n");

    return 0;
}