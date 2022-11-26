#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> // Macros erres
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define MAX_LINK 200
#define USAGE "Usage: %s directory_name\n"

#define RESET_COLOR    "\x1b[0m"
#define VERDE_T        "\x1b[32m"
#define AMARILLO_T "\x1b[33m"
#define CYAN_T     "\x1b[36m"

int is_directory(char path[]){

    struct stat info;
    
    if ( stat(path, &info) == -1){
        perror("Error in stat()\n");
        exit(EXIT_FAILURE);
    }

    if ((info.st_mode & S_IFMT) == S_IFDIR){
        return 1;
    }else{ 
        return 0;
    }
}
int main(int argc, char **argv){

   if (argc != 2){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
   
    }

    char *path = argv[1];

    if( !is_directory(path)){
        fprintf(stderr, "El fichero %s no es un directorio\n", path);
        exit(EXIT_FAILURE);
    }

    DIR *dir = opendir(path);

    if ( dir == NULL){
        perror("Error en opendir()");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    int sum = 0;

    while(( entry = readdir(dir)) != NULL){

        char file_path[strlen(path) + strlen(entry->d_name) +1];

        sprintf(file_path, "%s/%s",path, entry->d_name);

        struct stat info; 

        if ( lstat(file_path, &info) == -1){
            closedir(dir);
            perror("Error in stat()\n");
            exit(EXIT_FAILURE);
        }

        switch (info.st_mode & S_IFMT) { 
            case S_IFDIR:  
                printf("%s%s/%s\n",AMARILLO_T, file_path, RESET_COLOR);      
                break;
            case S_IFLNK:  

                char file_link[MAX_LINK];

                if ( readlink(file_path, file_link, MAX_LINK) == -1){
                    closedir(dir);
                    perror("Error reading link\n");
                    exit(EXIT_FAILURE);
                }
                
                printf("%s %s->%s %s \n",file_path, CYAN_T, RESET_COLOR, file_link);     
                break;

            case S_IFREG:  

                if ( (info.st_mode &  S_IXUSR) || (info.st_mode &  S_IXGRP)  || (info.st_mode &  S_IXOTH) ){
                       printf("%s%s*%s\n",file_path,VERDE_T, RESET_COLOR);
                } else{
                    printf("%s\n",file_path);
                }

                sum+= info.st_size;
                break;

            default:       
                printf("%s\n",file_path);       
                break;
        }

    }

    printf("Tamaño total de los ficheros regulares: %f kB\n", (float) sum/1024);  

    return 0;
}