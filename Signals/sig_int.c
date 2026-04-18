#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int count = 0;

void sig_handler(int sig){
	count++;
	printf("Caught SIG INT %d\r\n",count);
}

int main()
{
	int i;
	//TODO 1: Declare the variable of type struct sigaction
	struct sigaction sa,old_sa;

    	// 1. Initialize the structure
    	memset(&sa, 0, sizeof(sa));
    	memset(&sa, 0, sizeof(old_sa));
	//TODO 2: Nullify the variable
	//TODO 3: Register the handler
	sa.sa_handler = &sig_handler;
	sa.sa_flags = SA_RESETHAND;
	sigemptyset(&sa.sa_mask);
	
	if (sigaction(SIGINT, &sa, &old_sa) == -1) {
        	perror("sigaction failed");
        	return 1;
    	}

	for(i = 0; i < 20; i++)
	{
		//if(count == 1)
		//	if (sigaction(SIGINT, &old_sa, NULL) == -1) {
        	//		perror("old sigaction failed");
        	//		return 1;
		//	}
		printf("Hmmmmm\n");
		sleep(1);
	}
	return 0;
}
