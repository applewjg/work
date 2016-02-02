#include <stdio.h> /* printf */ 
#include <unistd.h> /* alarm, pause */ 
#include <sys/types.h> 
#include <signal.h> /* signal,kill */ 

typedef unsigned long long cycles_t; 

inline cycles_t currentcycles() 
{ 
    cycles_t result; 
    __asm__ __volatile__ ("rdtsc" : "=A" (result)); 
        
    return result; 
} 


int main(void) 
{ 
    cycles_t t1, t2; 
    t1 = currentcycles();
    sleep(1);
    t2 = currentcycles();
    printf("cpu MHz : %lld\n", (t2-t1)/1000000);
    return 0; 
} 
