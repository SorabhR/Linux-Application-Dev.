#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg)
{
	int i, res = 0; 

	/* Comment the line below to see the cancellation happening */
	//TODO 2: Disable the cancellation
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if (res != 0)
	{
		printf("Thread pthread_setcancelstate failed");
		pthread_exit(NULL);
	}
	printf("Thread_fuction is running\n");
	for(i = 0; i < 5; i++)
	{
		printf("Thread function is still running (%d) ...\n", i);
		sleep(1);
	}
	pthread_exit(NULL); // Never returns
}

int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;
	
	res = pthread_create(&a_thread, NULL, &thread_function, NULL);
	if (res != 0)
	{
		printf("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	sleep(3);
	printf("Cancelling thread ...\n");
	//TODO 1: Cancel the thread
	res = pthread_cancel(a_thread);
	if (res != 0)
	{
		printf("Thread Cancellation failed");
		exit(EXIT_FAILURE);
	}
	pthread_join(a_thread, &thread_result);
	if(thread_result != NULL)
		printf("Thread was cancelled\r\n");
	exit(EXIT_SUCCESS);
}
