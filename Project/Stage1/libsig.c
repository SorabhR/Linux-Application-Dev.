#include "sig.h"

int signal_register(int signum, void (*fun)(int), struct sigaction *oldact, 
                        sigset_t *sa_mask) 
{
    // TODO 1: Register the signal handler and save the current behaviour in oldact
    struct sigaction current_action;
    current_action.sa_handler = fun;
    current_action.sa_mask = *sa_mask;
    current_action.sa_flags = 0;
    if(sigaction(signum,&current_action,oldact) == -1){
	    perror("Handler registration failed");
	    return -1;
    }

    return 0;
}

int signal_deregister(int signum) 
{
    // TODO 2: Reset the behaviour to SIG_DFL
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIG_DFL;
    if(sigaction(signum,&sa,NULL) == -1){
	    perror("Default signal handler setting failed");
	    return -1;
    }
    return 0;
}

int signal_restore(int signum, struct sigaction *sa)
{
    // TODO 3: Restore the behaviour as per sa
    if(sigaction(signum,sa,NULL) == -1){
	    perror("Old registration failed");
	    return -1;
    }
    return 0;
}

int signal_ignore(int signum) 
{
    // TODO 4: Ignore the signal
    
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIG_IGN;
    if(sigaction(signum,&sa,NULL) == -1){
	    perror("Default signal handler setting failed");
	    return -1;
    }
    return 0;
}

int signal_send(pid_t pid, int signum)
{
    // TODO 5: Send the signal to the process 
    if(kill(pid,signum) == -1){
	    perror("Failed to send signal");
	    return -1;
    }
    return 0;
}
