#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "atomic.h"
#define NUM_THREADS     2000

int g_atomic_num = 0;

void * atomic_add_t(void * threadid) {
	int tid;
	tid = *(int*)threadid;
	int ret = atomic_add(&g_atomic_num, 1);
	printf("thread #%d! num = %d\n", tid, ret); 
	pthread_exit(NULL); 
}

int main(void) {
	pthread_t threads[NUM_THREADS];
	for ( int t = 0; t < NUM_THREADS; t++ ) {
		printf("in main: creating thread %d\n", t);
		int ret = pthread_create(&threads[t], NULL, atomic_add_t, (void*)&t);
	}
	for ( int t=0; t<NUM_THREADS; t++ ) { 
    	pthread_join(threads[t], NULL);
  	}
  	printf("res = %d\n", g_atomic_num);
    return 0;
}