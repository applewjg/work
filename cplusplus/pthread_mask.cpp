/* File: pthread_sigmask.c */
/* Date: 20140609 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

/* Simple error handling functions */

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static void * sig_thread(void *arg)
{
    sigset_t *set = (sigset_t *) arg;
    int s, sig;

    for (;;) {
        s = sigwait(set, &sig);
        if (s != 0)
            handle_error_en(s, "sigwait");
        printf("[sig_thread] Signal handling thread got signal %d\n", sig);
    }
    printf("exit sig_thread\n");
}

int main(int argc, char *argv[])
{
    pthread_t thread;
    sigset_t set;
    int s, sig;

    /* Block SIGINT; other threads created by main() will inherit
       a copy of the signal mask. */

    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    sigaddset(&set, SIGUSR1);
    s = pthread_sigmask(SIG_SETMASK, &set, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_sigmask");

    s = pthread_create(&thread, NULL, &sig_thread, (void *) &set);
    if (s != 0)
        handle_error_en(s, "pthread_create");

    /* Main thread carries on to create other threads and/or do
       other work */
    sleep(10);
    s = sigwait(&set, &sig);
    if (s != 0)
        handle_error_en(s, "sigwait");
    printf("[main] Signal handling thread got signal %d\n", sig);

    pause();            /* Dummy pause so we can test program */
}

