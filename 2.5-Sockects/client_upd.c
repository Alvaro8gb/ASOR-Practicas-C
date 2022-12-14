#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <sys/select.h>


#define handle_error_gai(code_error, msg) \
          do { fprintf(stderr, "Code error: %d msg: %s\n", code_error, gai_strerror(code_error)); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define USAGE  "Usage: %s dir_server puerto_server command\n"

int MAX_BUFFER = 150;

int main(int argc, char **argv){
 
    if(argc != 4){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    char msg[MAX_BUFFER];

    struct addrinfo * server_addr = NULL;
    struct addrinfo hints;
    char buf[3] = "c\n\0";

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC; // Tanto IPV4 como IPV6
    hints.ai_socktype = SOCK_DGRAM;

    int code_error = -1;
    if ( (code_error = getaddrinfo(argv[1], argv[2], &hints, &server_addr)) != 0) handle_error_gai(code_error, "Error in getaddrinfo()");

    int udp_sd = -1;
    if( (udp_sd = socket(server_addr->ai_family, server_addr->ai_socktype, server_addr->ai_protocol)) == -1) handle_error("Error in socket()"); 
    
    if( sendto(udp_sd, argv[3], sizeof(argv[3]), 0, server_addr->ai_addr, server_addr->ai_addrlen) == -1) handle_error("Error in sendto()");

    struct sockaddr_storage src_addr;
    socklen_t src_addrlen = sizeof(src_addr);

    ssize_t bytes_read = 0;
    if( (bytes_read = recvfrom(udp_sd, msg, MAX_BUFFER, 0, (struct sockaddr *) &src_addr, &src_addrlen)) == -1) handle_error("Error in recvfrom()");

    char host[NI_MAXHOST], serv[NI_MAXSERV];
    if((code_error = getnameinfo((struct sockaddr *) &src_addr, src_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV)) != 0) handle_error_gai(code_error, "Error in getnameinfo()");

    close(udp_sd);

    printf("Client recibed from dir: %s && port: %s , msg: \n", host, serv);

    write(STDOUT_FILENO, msg, bytes_read);
    write(STDOUT_FILENO, "\n",1);

    return 0;

}