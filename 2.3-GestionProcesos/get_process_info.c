
#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)
 
#define MAX_PATH 200

int main(){

    pid_t pid = getpid();
    printf("PID (Process Identifier): %d\n", getpid());
    printf("PPID (Parent Process Identifier): %d\n", getppid());
    printf("PGRP (Process Group) or PGID (Process Group Identifier): %d\n", getpgid(pid)); 
    printf("SID (Sesion Identifier): %d\n",getsid(pid)); 

    char buff[MAX_PATH];
    if ( getcwd(buff, MAX_PATH) == NULL) handle_error("Error in getcwd () ");

    printf("Working directory path: %s\n", buff);
    
    struct rlimit limits;
    if ( getrlimit(RLIMIT_NOFILE, &limits) == -1) handle_error("Error in getrlimit()");
 

    printf("Numero maximo de ficheros posible: %ld\n", limits.rlim_max);
    printf("Numero maximo de ficheros recomendado: %ld\n", limits.rlim_cur);


    return 0;
}