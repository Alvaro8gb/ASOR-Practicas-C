#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h> // Macros erres
#include <stdio.h>
#include <sys/sysmacros.h>
#include <time.h>

int main(){

    struct stat info;

    //lstat igual qu stat pero lo senlaces simbolicos devuelve la info del enlace y no de lo apuntado.

    if ( lstat("stat.c", &info) == -1){
        perror("Error in stat()\n");
        exit(EXIT_FAILURE);
    }

    printf("Inodo number %ld\n", info.st_ino);
    printf("Major number %d\n", major(info.st_dev));
    printf("Minor number %d\n", minor(info.st_dev));
    
    printf("File type:                ");

    switch (info.st_mode & S_IFMT) { // solo la parte del tipo de fichero
        case S_IFDIR:  printf("directory\n");      break;
        case S_IFLNK:  printf("symlink\n");        break;
        case S_IFREG:  printf("regular file\n");   break;
        default:       printf("other\n");          break;
    }

    printf("Last file access:         %s", ctime(&info.st_atime));
    printf("Last file modification:   %s", ctime(&info.st_mtime));


    return 0;
}