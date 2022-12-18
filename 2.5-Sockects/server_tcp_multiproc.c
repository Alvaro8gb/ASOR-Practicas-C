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

int BUFF_MESSAGE = 150;

int main(int argc, char **argv){
 
    if(argc != 3){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Trying to create service TCP in dir: %s && port %s\n", argv[1], argv[2]);

    struct addrinfo * result = NULL;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC; // Tanto IPV4 como IPV6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int code_error = -1;
    if ( (code_error = getaddrinfo(argv[1], argv[2], &hints, &result)) != 0) handle_error_gai(code_error, "Error in getaddrinfo()");

    int tcp_sd = -1;

    if( (tcp_sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) ==-1) handle_error("Error in socket()");

    if( bind(tcp_sd, (struct sockaddr *) result->ai_addr, result->ai_addrlen) == -1) handle_error("Error in bind()");
    freeaddrinfo(result);

    if( listen(tcp_sd, N_BACKLOG) == -1 ) handle_error("Error in listen()");

    printf("Serving with protocol TCP in port: %s\n", argv[2]);

    ssize_t bytes_read;
    char buf[BUFF_MESSAGE];

    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    char host[NI_MAXHOST], serv[NI_MAXSERV];
    int client_sd;

    do{

        client_sd = accept(tcp_sd, (struct sockaddr *) &client_addr, &client_addrlen ) ;
        if ( client_sd == -1) handle_error("Error in accept()");
        
        int ret = fork();

        if( ret == 0){ // Hijos procesan las conexiones

            if((code_error = getnameinfo((struct sockaddr *) &client_addr, client_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV)) != 0) handle_error_gai(code_error,"Error in getnameinfo()");
            
            printf("Conexión desde %s  %s pid: %d\n", host, serv, getpid());
            
            while( (bytes_read = recv(client_sd, buf, BUFF_MESSAGE, 0)) > 0){
                
                if( send(client_sd, buf, bytes_read, 0) == -1) handle_error("Error in sendto()");

            }

            close(client_sd);
            printf("Conexión terminada desde %s %s pid: %d\n", host, serv, getpid());

            exit(EXIT_SUCCESS);
        
        }else{ // Padre 
            close(client_sd);
        }

    }while(1);

    close(tcp_sd);
    printf("Server in dir: %s && port: %s closed\n", argv[1], argv[2]);

    return 0;

}