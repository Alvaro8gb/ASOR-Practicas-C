// Ejercicio 2 (1 punto). Escribe un programa servidor UDP que escuche en una dirección (IPv4 o IPv6
// en cualquier formato) y puerto dados como argumentos. Recibirá del cliente una ruta de fichero y le
// devolverá una cadena con el tipo del fichero (regular, directorio, enlace, fifo u otro). Además, al
// recibir cada mensaje imprimirá en el terminal la dirección y el puerto del cliente.

//Escribe un programa servidor TCP que devuelva la dirección del cliente al
//recibir cualquier mensaje. El programa se ejecutará de la siguiente manera:
//$ ./tcp <dir IPv4 o IPv6 en cualquier formato> <puerto>
//Por ejemplo:
//$ ./tcp :: 7777

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int MAX_BUFFER = 2000;

#define handle_gai_error(code) \
           do { fprintf(stderr, "error: %s\n", gai_strerror(code)); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char ** argv){

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <dir IPv4 o IPv6 en cualquier formato> <puerto>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int code_error = -1, fd;
    char host[NI_MAXHOST], serv[NI_MAXSERV];
    int nread = 0, nwrite = 0, tam = 0;

    struct addrinfo hints;
    struct addrinfo *result;
    char buf[MAX_BUFFER];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* udp socket */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_flags = AI_PASSIVE;


    if ( code_error = getaddrinfo(argv[1], argv[2], &hints, &result ) == -1) handle_gai_error(code_error);

    if( (fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) handle_error("Error in sockect()");

    if( bind(fd, result->ai_addr, result->ai_addrlen) == -1) handle_error("Error in bind()");

    freeaddrinfo(result);

    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);
    struct stat statbuf;
    

    while (1){

        nread = recvfrom(fd, buf, MAX_BUFFER, 0, (struct sockaddr *) &peer_addr, &peer_addr_len);

        if( code_error = getnameinfo( (struct sockaddr *) &peer_addr, peer_addr_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST )) handle_gai_error(code_error);

        printf("Cliente %s:%s \n", host, serv);

        buf[nread-1] = '\0';

        if ( lstat(buf, &statbuf) != -1){
           switch (statbuf.st_mode & S_IFMT) {
           case S_IFCHR:  sprintf(buf, "character device\n");  tam = 18;      break;
           case S_IFBLK:  sprintf(buf, "block device\n");      tam = 14;      break;
           case S_IFDIR:  sprintf(buf, "directory\n");         tam = 11 ;      break;
           case S_IFIFO:  sprintf(buf,"FIFO/pipe\n");          tam = 11;     break;
           case S_IFLNK:  sprintf(buf,"symlink\n");            tam = 9;     break;
           case S_IFREG:  sprintf(buf,"regular file\n");       tam = 14;     break;
           case S_IFSOCK: sprintf(buf,"socket\n");             tam = 8;     break;
           default:       sprintf(buf,"unknown?\n");           tam = 10;     break;
           }

        }else{
            sprintf(buf, "error in lstat()");
        }

        nwrite = sendto(fd, buf, tam, 0, (struct sockaddr *) &peer_addr, peer_addr_len );

    }
    
    close(fd);

}