#include <pthread.h>
#include <stdio.h>

struct char_print_parms /* Parameters to print_function. */
{
	char character; /* The character to print. */
	int count; /* The number of times to print it. */
};

void *char_print(void* parameters)
{
	struct char_print_parms *p = (struct char_print_parms *)parameters;
	int i;

	for (i = 0; i < p->count; ++i)
	{
		fputc(p->character, stderr);
		fflush(stderr);
		sleep(10 / p->count);
	}

	return NULL;
}

int main()
{
	pthread_t thread1_id;
	pthread_t thread2_id;
	struct char_print_parms thread1_args;
	struct char_print_parms thread2_args;
	
	thread1_args.character = 'x';
	thread1_args.count = 3;
	thread2_args.character = 'o';
	thread2_args.count = 10;


	printf("Printing ... ");
	fflush(stdout); // TRY: Comment it out & see the fun

	//TODO 1: Create a new thread to print 3 ’x’s
	pthread_create(&thread1_id, NULL, &char_print, (void*) &thread1_args);
	//TODO 2: Create a new thread to print 10 o’s
	pthread_create(&thread2_id, NULL, &char_print, (void*) &thread2_args);

	//TODO 3: Join thread with id thread1_id
	pthread_join(thread1_id, NULL);
	//TODO 4: Join thread with id thread2_id
	pthread_join(thread2_id, NULL);

	printf(" done\n");

	return 0;
}
