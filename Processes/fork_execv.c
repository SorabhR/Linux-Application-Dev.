#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	int ret;
	// TODO 1: Declare the argument list for "ls -l /"
    	char* temp[3];
	temp[0] = "ls";
	temp[1] = "-l";
	temp[2] = NULL;
	
	// TODO 2: Create the process with fork
	ret = fork();
	if (ret == 0)
	{
    	// TODO 3: Invoke execv in the child process with above argument list
		execvp("ls",temp);
		printf("\nThis will not print, if all ok\n");
	}
	else{
		sleep(1);
	}

    return 0;
}
