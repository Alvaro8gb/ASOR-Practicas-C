#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>



int main(){


    if ( setuid(0) == -1){
        perror(NULL);
        printf("Codigo de error : %d \t Mensaje asociado: %s\n", errno, strerror(errno));
    }


    return 1;
}