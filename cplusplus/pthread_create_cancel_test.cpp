#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_MAX 4

pthread_mutex_t mutex;
pthread_t thread[THREAD_MAX];

static int tries;
static int started;

void print_it(void *arg) {
	pthread_t tid;
	tid = pthread_self();
	printf("Thread %lx was canceled on its %d try.\n", tid, *(int*)arg);
}

void *Search_Num(void * parg) {
	int arg = *(int *)parg;
	pthread_t tid;
	int num;
	int k = 0, h = 0, j;
	int ntries;
	tid = pthread_self();

	/*while(pthread_mutex_trylock(&mutex) == EBUSY)
	 {
	 printf("**************busy****************\n");
	 pthread_testcancel();
	 }*/
	srand(arg);
	num = rand() & 0xFFFFFF;
	//pthread_mutex_unlock(&mutex);

	printf("thread num %lx\n", tid);

	ntries = 0;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	pthread_cleanup_push(print_it,(void *)&ntries);

	while (1) {
		num = (num + 1) & 0xffffff;
		ntries++;

		if (arg == num) {

			//只允许一个线程操作此处
			while (pthread_mutex_trylock(&mutex) == EBUSY) {
				//一个线程操作后其余线程进入次循环挂起，等待pthread_cancel函数发送cancel信号终止线程
				k++;
				if (k == 10000) {
					printf("----------2busy2-----------\n");
				}

				pthread_testcancel();
			}
			tries = ntries;

			//pthread_mutex_unlock(&mutex);
			//如果加上这句话，将会有好几个线程找到主函数中设定的值pid
			printf("Thread %lx found the number!\n", tid);

			for (j = 0; j < THREAD_MAX; j++) {
				if (thread[j] != tid) {
					pthread_cancel(thread[j]);
				}
			}

			break;
		}
		if (ntries % 100 == 0) {
			h++;
			/*线程阻塞，其他线程争夺资源，或者是等待pthread_cancel函数发送cancel信号终止线程*/
			pthread_testcancel();
			/*这是为了弄明白pthread_testcancel函数的作用而设置的代码段*/
			if (h == 10000) {
				h = 0;
				printf("----------thread num %lx-------------\n", tid);
			}
		}
	}
	pthread_cleanup_pop(0);
	return (void *) 0;
}

int main() {
	int i, pid;

	pid = getpid(); //设置要查找的数

	pthread_mutex_init(&mutex, NULL);
	printf("Search the num of %d\n", pid);
	for (started = 0; started < THREAD_MAX; started++) {
		pthread_create(&thread[started], NULL, Search_Num, (void *) &pid);
	}

	for (i = 0; i < THREAD_MAX; i++) {
		printf("-----------i = %d--------------\n", i);
		pthread_join(thread[i], NULL);
	}
	printf("It took %d tries ot find the number!\n", tries);
	return 0;
}