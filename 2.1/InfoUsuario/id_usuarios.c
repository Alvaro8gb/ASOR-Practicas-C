#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <pwd.h>
#include <errno.h>
#include <string.h>


int main(){

    uid_t id_real = getuid();

    printf("Usuario real %d \n", id_real);
    printf("Usuario efectivo %d \n", geteuid());

    //  si el id real y el efectivo es diferente seguro q tienen el bit setuid activo.


    errno = 0;
    struct passwd *info  = getpwuid(id_real);
    if ( errno == 0){


          printf("Nombre de usuario %s \n", info->pw_name);
          printf("Directorio home %s \n", info->pw_dir);
          printf("Descripcion del usuario %s \n", info->pw_gecos);

    }
    else{

        printf("Error leyendo información sobre el usuario\n");
        printf("Codigo de error : %d \t Mensaje asociado: %s\n", errno, strerror(errno));

    }

  

    return 0 ;
}