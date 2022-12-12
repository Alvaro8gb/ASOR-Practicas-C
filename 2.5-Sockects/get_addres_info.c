#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define USAGE  "Usage: %s addr_dir\n"

socklen_t MAX_HOST = 200;

// Las familias 2 y 10 son AF_INET y AF_INET6, respectivamente (ver socket.h)
// Los tipos 1, 2, 3 son SOCK_STREAM, SOCK_DGRAM y SOCK_RAW, respectivamente 

int main(int argc, char **argv){
 
    if(argc != 2){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo * info_array = NULL;
    
    //Specifying  hints  as  NULL  is  equivalent  to setting ai_socktype and
    //   ai_protocol  to  0;   ai_family   to   AF_UNSPEC;   and   ai_flags   to
    //   (AI_V4MAPPED | AI_ADDRCONFIG)

    if ( getaddrinfo(argv[1], NULL, NULL, &info_array) != 0) handle_error("Error in getaddrinfo()");

    printf("host\tfamlily\tsocktype\n");
    char host[MAX_HOST];
    for (struct addrinfo *rp = info_array; rp != NULL; rp = rp->ai_next) {

        if ( getnameinfo(rp->ai_addr, rp->ai_addrlen, host, MAX_HOST, NULL, 0, NI_NUMERICHOST) != 0) handle_error("Error in getnameinfo()");
            
        printf("%s\t%d\t%d\n",host, rp->ai_family, rp->ai_socktype);
    }

    return 0;
}
 