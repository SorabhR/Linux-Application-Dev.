#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sig.h"

void sig(int signum)
{
    printf("Received signal\n");
}

int main()
{
    // TODO 1: Register the handler for SIGINT
    struct sigaction old_act;
    sigset_t custom_mask;
    sigemptyset(&custom_mask);

    if(signal_register(SIGINT,sig,&old_act,&custom_mask) == -1){
	    //printf("Handler failed\r\n");
	    perror("Handler registration failed");
    }
    else
	    printf("Registration done\r\n");
    pause();
    return 0;
}
