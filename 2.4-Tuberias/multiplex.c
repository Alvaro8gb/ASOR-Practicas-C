#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)

const int MAX_SECONDS = 15;
const int TAM_BUFFER = 256;

void read_input(int fd){
    char buff[TAM_BUFFER];
    int num_read = 0;

    do{
        num_read = read(fd, buff, TAM_BUFFER);

        if( num_read == -1) handle_error("Error in read()");
          
        if ( write(STDOUT_FILENO, buff, num_read) == -1) handle_error("Error in write()");
        
    }while(num_read == TAM_BUFFER);

    close(fd);
}

int open_readeable_pipe(char path_pipe[]){

    int fd = open(path_pipe, O_RDONLY | O_NONBLOCK, 00400 );

    if( fd == -1) handle_error("Error opening pipe");

    return fd;
}

int main(){

    fd_set rfds;
    struct timeval tv;
    int retval;    

    int fd1 = open_readeable_pipe("named_pipe_1");
    int fd2 = open_readeable_pipe("named_pipe_2");

    do{
        
        FD_ZERO(&rfds);
        FD_SET(fd1, &rfds);
        FD_SET(fd2, &rfds);

        tv.tv_sec = MAX_SECONDS;
        tv.tv_usec = 0;

        retval = select(fd2+1, &rfds, NULL, NULL, &tv);
            
        if (retval == -1){
            perror("select()");
            exit(EXIT_FAILURE);
        }
           
        if(FD_ISSET(fd1, &rfds)){
            printf("Reading named_pipe_1:\n");
            
            read_input(fd1);
    
            int fd1 = open_readeable_pipe("named_pipe_1");
        }
        
        if(FD_ISSET(fd2, &rfds)){
            printf("Reading named_pipe_2\n");

            read_input(fd2);

            int fd2 = open_readeable_pipe("named_pipe_2");
        }
            
    }while(retval!=0);

    printf("No data within %d seconds.\n", MAX_SECONDS);

    return 0;
}