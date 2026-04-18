#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int status, i;

    if (fork() == 0)
    {
        for (i = 0; i < 20; i++)
        {
            sleep(1);
        }
    }
    else
    {
	if(fork() == 0){
		sleep(20);
	}
	else{	
        	// TODO 1: Wait for the child process termination
        	pid_t child_pid = wait(&status);
		printf("Child done is %d\r\n",child_pid);
		// TODO 2: Print the exit code, if process exited normally
		if(status == 0){
			printf("Exited successfully %d\n",status);
		}
		else
			printf("Abnormal exit\n");
    	}
    }
    return 0;
}
