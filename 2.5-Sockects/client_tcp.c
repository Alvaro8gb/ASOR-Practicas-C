#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>

#define handle_error_gai(code_error, msg) \
          do { fprintf(stderr, "Code error: %d msg: %s : %s\n", code_error, msg, gai_strerror(code_error)); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define USAGE  "Usage: %s dir puerto\n"
#define N_BACKLOG 16

int MAX_BUF = 150;

int main(int argc, char **argv){
 
    if(argc != 3){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo * result = NULL;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC; // Tanto IPV4 como IPV6
    hints.ai_socktype = SOCK_STREAM;

    int code_error = -1;
    if ( (code_error = getaddrinfo(argv[1], argv[2], &hints, &result)) != 0) handle_error_gai(code_error, "Error in getaddrinfo()");

    int tcp_sd = -1;
    if( (tcp_sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) handle_error("Error in socket()");

    if ( connect(tcp_sd, result->ai_addr, result->ai_addrlen ) == -1) handle_error("Error in conect()");

    freeaddrinfo(result);

    char host[NI_MAXHOST], serv[NI_MAXSERV];
    if((code_error = getnameinfo(result->ai_addr, result->ai_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV)) != 0) handle_error_gai(code_error,"Error in getnameinfo()");
        
    printf("Conexión desde %s  %s\n", host, serv);

    char msg[MAX_BUF];
    size_t bytes_read = 0;

    do{
        bytes_read = read(STDIN_FILENO, &msg, MAX_BUF);

        if(bytes_read == -1) handle_error("Error in read()");

        if( msg[0] == 'Q' && msg[1] == '\n' && bytes_read == 2){
            close(tcp_sd);
            exit(EXIT_SUCCESS);
        }
        
        if(send(tcp_sd, msg, bytes_read, 0) == -1) handle_error("Error in send()");

        if((bytes_read = recv(tcp_sd, msg, MAX_BUF, 0)) == -1) handle_error("Error in recv()");

        write(STDOUT_FILENO, msg, bytes_read);

    }while(1);

}