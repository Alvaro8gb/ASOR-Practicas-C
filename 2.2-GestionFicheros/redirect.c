#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> // Macros erres
#include <stdio.h>
#include <unistd.h>

#define USAGE "Usage: %s file_name\n"

int main(int argc, char **argv){

     if (argc != 2){
          fprintf(stderr, USAGE, argv[0]);
          exit(EXIT_FAILURE);
     }

     char *path = argv[1];
     int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC);

     if ( fd == -1 ){
          perror("Error al crear el fichero");
          exit(EXIT_FAILURE);
     }

     if ( dup2(fd,1) == -1){
          close(fd);
          perror("Error en dup()\n");
          exit(EXIT_FAILURE);
     }

     write(1,"Hola\n",5);
     printf("HOla caracola\n");
     close(fd);

    return 0;
}