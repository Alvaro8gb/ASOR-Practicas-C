#include <stdio.h>
#include <time.h>

#include <sys/time.h>
#include <stdlib.h>

int time_seconds(){

    time_t seconds = time(NULL);

    if ( seconds != -1){
        printf("Seconds since EPOCH: %ld\n", seconds);
        return seconds;

    }else{
        printf("Error using time(3)");
        return -1;
    }

}

int loop_to_size(){
    int sum =0;
    
    struct timeval start, end; 
    
    if( gettimeofday(&start, NULL) == -1){
        printf("Error getting time\n");
        return 1;
    }

    for ( int i; i < 1000000; i++){
        sum +=i;
    }

    if( gettimeofday(&end, NULL) == -1){
        printf("Error getting time\n");
        return 1;
    }
    
    printf("Time to execute the loop is : %ld  microseconds \n",end.tv_usec - start.tv_usec);
}
void show_current_year(){

    time_t seconds = time_seconds();

    if (seconds != -1){
    
        struct tm *t = localtime(&seconds);
        
        printf("Current year: %d \n", t->tm_year + 1900);

        int max_format = 200;
        char * time_format = malloc(max_format);

        if ( strftime(time_format, max_format,"%A, %d de %B de %G, %R" , t) > 0){
            printf("Nuestro formato: %s\n", time_format);

        }else{
            printf("Error showing time format\n");
        }

        free(time_format);

    }

}
int main(){

    loop_to_size();
    show_current_year();


    return 0 ;
}