
#include <sched.h>
#include <sys/sysmacros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error(msg) \
          do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(){
    
    int sched = sched_getscheduler(0);

    if ( sched == -1)  handle_error("Error in sched_getscheduler()");

    
    char policy[12];
    switch(sched){
        case SCHED_OTHER:
        sprintf(policy, "SCHED_OTHER"); break;
        case SCHED_FIFO:
        sprintf(policy, "SCHED_FIFO"); break;
        case SCHED_RR:
        sprintf(policy, "SCHED_RR"); break;
        default:
        sprintf(policy, "OTHER"); break;
    }

    printf("Policy : %s \n", policy);
    printf("Max: %d\n", sched_get_priority_max(sched));
    printf("Max: %d\n", sched_get_priority_min(sched));

    struct sched_param p;

    if( sched_getparam(0, &p) == -1) handle_error("Error en sched_getparam()");

    printf("Prioridad del proceso: %d\n", p.sched_priority);

    return 0;
}