
#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)
 

int main(){

    pid_t pid = fork();

    switch (pid) {
        case -1:
            handle_error("Error in fork()");
        case 0: //Hijo
            waitpid(getppid(), NULL, 0);
            printf("Hijo %i (padre: %i)\n", getpid(), getppid());
            break;
        
        default:
            printf("Padre %i (hijo: %i)\n", getpid(), pid);
            exit(EXIT_SUCCESS);
    }

    // Demon set up
    
    if ( chdir("/tmp") == -1)  handle_error("Error in chdir()");
    

    if ( setsid() == -1) handle_error("Error in setsid()");
    
    // Service logic
    
    if ( execl("/home/ab-82/Desktop/ASOR-Practicas-C/2.3-GestionFicheros/get_process_info.a","get_process_info", NULL) == -1)
        handle_error("Error in execl()");
    

    return 0;

}