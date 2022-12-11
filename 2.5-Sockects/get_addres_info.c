#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)


#define USAGE  "Usage: %s seconds_before_delete\n"

socklen_t MAX_HOST = 200;

int main(int argc, char **argv){
 
    if(argc != 2){
        fprintf(stderr, USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo * info_array = NULL;

    if ( getaddrinfo(argv[1], NULL, NULL, &info_array) != 0) handle_error("Error in getaddrinfo()");

    char host[MAX_HOST];
    for (struct addrinfo *rp = info_array; rp != NULL; rp = rp->ai_next) {

        if ( getnameinfo(rp->ai_addr, rp->ai_addrlen, host, MAX_HOST, NULL, 0, NI_NUMERICHOST) != 0) handle_error("Error in getnameinfo()");
            
        printf(" %s %d %d \n",host, rp->ai_family, rp->ai_socktype);
    }


    return 0;
}
 