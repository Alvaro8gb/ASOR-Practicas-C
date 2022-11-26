#include <unistd.h>
#include <stdlib.h> // Macros erres
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

#define USAGE "Usage: %s path_to_a_regular_file\n"

int main(int argc, char **argv){

   if (argc != 2){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
   }

    struct stat info;
    char *path = argv[1];

    printf("Working path: %s\n", argv[1]);

    if ( lstat(path, &info) == -1){
        perror("Error in stat()\n");
        exit(EXIT_FAILURE);
    }

    if ( (info.st_mode & S_IFMT) != S_IFREG){
        perror("No es fichero regular\n");
        exit(EXIT_FAILURE);
    }

    char new_path_hard[strlen(path)+ 5];
    char new_path_symbolic[strlen(path)+ 5];

    sprintf(new_path_hard, "%s.hard", path);
    sprintf(new_path_symbolic,"%s.sym", path);

    link(new_path_hard, path);
    symlink(new_path_symbolic, path);

    return 0;
}