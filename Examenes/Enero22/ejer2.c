//Escribe un programa servidor TCP que devuelva la dirección del cliente al
//recibir cualquier mensaje. El programa se ejecutará de la siguiente manera:
//$ ./tcp <dir IPv4 o IPv6 en cualquier formato> <puerto>
//Por ejemplo:
//$ ./tcp :: 7777

#include <sys/types.h>
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
    char host[NI_MAXHOST];
    int nread = 0;

    struct addrinfo hints;
    struct addrinfo *result;
    char buf[MAX_BUFFER];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* tcp socket */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_flags = AI_PASSIVE;


    if ( code_error = getaddrinfo(argv[1], argv[2], &hints, &result ) == -1) handle_gai_error(code_error);

    if( (fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) handle_error("Error in sockect()");

    if( bind(fd, result->ai_addr, result->ai_addrlen) == -1) handle_error("Error in bind()");

    freeaddrinfo(result);

    if( listen(fd, 16) == -1) handle_error("Error in listen");

    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);

    while (1){
        if( accept(fd, (struct sockaddr *) &peer_addr, &peer_addr_len) == -1) handle_error("Error in acept");

        nread = recv(fd, buf, MAX_BUFFER, 0);

        if( code_error = getnameinfo( (struct sockaddr *) &peer_addr, peer_addr_len, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST )) handle_gai_error(code_error);

        printf("CLiente %s\n",host);

    }
    
    close(fd);

}