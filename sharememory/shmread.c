#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include "shmdata.h"

int main() {
    int running = 1;
    void *shm = NULL;
    struct shared_use_st *shared;
    int shmid;
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);


}
