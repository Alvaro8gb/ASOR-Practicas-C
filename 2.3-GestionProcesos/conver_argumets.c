#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define USAGE "Usage: %s arguments\n"

int main(int argc, char ** argv){

    if ( argc == 1){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }  

    int tam = 0;

    for (int i = 1; i < argc ; i++){
        tam+= strlen(argv[i]);
    }
    
    char * command = malloc(tam + (argc-1));
    int offset = 0;

    for (int i = 1; i < argc ; i++){
         
        sprintf(&command[offset],"%s ", argv[i]);
        offset += strlen(argv[i]) + 1;

    }

    printf("Argumetns : %s \n", command);
    return 0;
}