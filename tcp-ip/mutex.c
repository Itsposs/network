
#include "optional.h"

#define NUM_THREAD 100

void* thread_inc(void *arg);
void* thread_des(void *arg);

long long num = 0;   // 64bit
pthread_mutex_t mutex;

int main(int argc, char *argv[]) {
	int i = 0;
	pthread_t thread_id[NUM_THREAD];
	
	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < NUM_THREAD; i++) {
		if (i % 2) 
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}

	for (i = 0; i < NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL);
	printf("result: %lld \n", num);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void* thread_inc(void *arg) {
	int i;
	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < 500000; i++)
		num += 1;
	pthread_mutex_destroy(&mutex);
	return NULL;
}

void* thread_des(void *arg) {
	int i;
	for (i = 0; i < 500000; i++) {
		pthread_mutex_init(&mutex, NULL);
		num -= 1;
		pthread_mutex_destroy(&mutex);
	}
	return NULL;
}
