#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


int main(){

   for(int i = 1; i<255; i++){
         printf("Codigo de error :%d \t Mensaje asociado: %s\n",i, strerror(i));
   }

    return 0;
}