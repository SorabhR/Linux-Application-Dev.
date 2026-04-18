#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sig_atomic_t sigcount = 0;

void handler(int sigint)
{
	sigcount++;
}

int main()
{
	int i;
	struct sigaction sa;
	sigset_t mask, oldmask;
	sigset_t pending;

	//TODO 1: Nullify the sigaction data structure, 
	memset(&sa,0,sizeof(sa));
	// and register the handler 
	sa.sa_handler = &handler;

	sigaction(SIGINT,&sa,NULL);
	//sigemptyset(&mask);
	//sigaddset(&mask,SIGINT);

	for(i = 0; i < 20; i++)
	{
		if (i == 2)
		{
			//TODO 2: Mask the SIGINT with following steps
			// Clear the mask with sigemptyset, add SIGINT with sigaddset and finally
			// mask it using sigprocmask. Make sure to get the previous mask in oldmask
			sigemptyset(&mask);
			sigaddset(&mask,SIGINT);
			sigprocmask(SIG_BLOCK, &mask, &oldmask);
		}
		if (i == 10)
		{
			//TODO 3: Restore the mask with sigprocmask
			sigpending(&pending);
			for (int i = 1; i < NSIG; i++) {
        			if (sigismember(&pending, i)) {
            				// strsignal(i) gives the name of the signal (e.g., "Interrupt")
            				printf("%d (%s) ", i, strsignal(i));
        			}
    			}
			sigprocmask(SIG_SETMASK, &oldmask, NULL);
		}
			
		printf("Hmmmmm sigatomic = %d\n", sigcount);
		sleep(1);
	}
	return 0;
}
