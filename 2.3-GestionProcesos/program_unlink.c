#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)
 
#define USAGE  "Usage: %s seconds_before_delete\n"
 
static void handler(int signal){
 
   //printf("Borrado pausado\n"); // No se puede usar esta funcion aqui. solo funciones descritas en man signal-safety
   write(0, "Borrado pausado\n", 17);
 
   sigset_t block_signals;
   if( sigemptyset(&block_signals) == -1) handle_error("Error in sigemptyset()");
 
   if( sigaddset(&block_signals, signal) == -1 ) handle_error("Error adding signal to set");
 
   sigsuspend(&block_signals);
 
}
 
int main(int argc, char **argv){
 
   if(argc != 2){
       fprintf(stderr, USAGE, argv[0]);
       exit(EXIT_FAILURE);
   }
 
   // Activate Handler
   struct sigaction sa;
 
   sa.sa_flags = SA_SIGINFO;
   if(sigemptyset(&sa.sa_mask) == -1) handle_error("Error in sigemptyset()");
   sa.sa_sigaction = (void *)  handler;
  
   if (sigaction(SIGUSR1, &sa, NULL) == -1) handle_error("Error in sigaction(SIGUSR1)");
 
   int segs = atoi(argv[1]);
 
   if(segs == 0) handle_error("Invalid seconds to suspend");
 
   printf("PID : %d is going to delete %s in %d seconds\n", getpid(),argv[0], segs);
 
   sleep(segs);
  
   if ( unlink(argv[0]) == -1) handle_error("Error unlinking");
 
}

