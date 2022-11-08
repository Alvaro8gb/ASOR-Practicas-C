#include <stdio.h>
#include <sys/utsname.h>

#include <unistd.h>
#include <limits.h>

void sys_error(long ret, char message[], char messageError[]){

    if(ret!=-1){
        printf("%s %ld \n",message, ret);

    }
    else{
        printf("%s\n",messageError);    
    }

}

int path_info(char path[]){

    sys_error(pathconf(path,_PC_LINK_MAX),"EL numero máximo de enlaces","Error viendo el numero máximo de enlaces");
    sys_error(pathconf(path,_PC_PATH_MAX),"Tamaño máximo de una ruta","Error viendo el tamaño máximo de una ruta");
    sys_error(pathconf(path,_PC_NAME_MAX),"Tamaño máximo del nombre de un fichero","Error viendo el tamaño maximo del nombre de un fichero");


    return 0;
}

int sysconf_info(){


    sys_error(sysconf(_SC_ARG_MAX), "Numero maximo de argumentos", "Error al sacar le numero maximo de argumentos");
    sys_error(sysconf(_SC_CHILD_MAX), "Numero maximo de hijos", "Error al sacar le numero maximo de hijos");
    sys_error(sysconf(_SC_OPEN_MAX), "Numero maximo de ficheros abiertos", "Error al sacar el numero maximo de ficheros abiertos");
    

    return 0;
}
int uname_info(){
    struct utsname s_info;


    if ( uname(&s_info)==0){

            printf("Operating sistema name: %s \n", s_info.sysname);
            printf("Node name: %s \n", s_info.nodename);
            printf("Release: %s \n", s_info.release);
            printf("Version: %s \n", s_info.version);
            printf("Machine: %s \n", s_info.machine);
            printf("Domain name: %s \n", s_info.__domainname);

        }else{
            printf("Error en uname viendo la info del sistema\n");
        }


    return 0;
}
int main(){

    uname_info();
    sysconf_info();
    path_info("/");

    return 0;
}