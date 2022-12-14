#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>

#define handle_error_gai(code_error, msg) \
          do { fprintf(stderr, "Code error: %d msg: %s\n", code_error, gai_strerror(code_error)); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define USAGE  "Usage: %s dir puerto\n"

int BUFF_MESSAGE = 150;

int main(int argc, char **argv){
 
    if(argc != 3){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    char msg[BUFF_MESSAGE];

    printf("Trying to create service UDP in dir: %s && port %s\n", argv[1], argv[2]);

    struct addrinfo * result = NULL;
    struct addrinfo hints;
    char buf[3] = "c\n\0";

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC; // Tanto IPV4 como IPV6
    hints.ai_socktype = SOCK_STREAM;

    int code_error = -1;
    if ( (code_error = getaddrinfo(argv[1], argv[2], &hints, &result)) != 0) handle_error_gai(code_error, "Error in getaddrinfo()");

    int udp_sd = -1;

    if( (udp_sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) ==-1) handle_error("Error in socket()");

    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof(addr);
    char host[NI_MAXHOST], serv[NI_MAXSERV];

    if( bind(udp_sd, (struct sockaddr *) result->ai_addr, result->ai_addrlen) == -1) handle_error("Error in bind()");

    if( listen(udp_sd, 40) == -1 ) handle_error("Error in listen()");

    ssize_t bytes_read;
    int bytes_to_client = 0;

    printf("Serving with protocol TCP in port: %s\n", argv[2]);

    do{
        bytes_read = recvfrom(udp_sd, buf, 2, 0, (struct sockaddr *) &addr, &addrlen);
        
        if( bytes_read == -1) handle_error("Error redinng from client");

        if((code_error = getnameinfo((struct sockaddr *) &addr, addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV)) != 0) handle_error_gai(code_error,"Error in getnameinfo()");
        
        printf("%ld bytes de %s:%s\n", bytes_read, host, serv);

        if( sendto(udp_sd, msg, bytes_to_client, 0, (struct sockaddr *) &addr, addrlen) == -1) handle_error("Error in sendto()");

    }while(buf[0] != 'q');

    close(udp_sd);
    printf("Server in dir: %s && port: %s closed\n", argv[1], argv[2]);

    return 0;

}