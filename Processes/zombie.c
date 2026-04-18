#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	int cpid, status;
	cpid = fork();
	if(cpid == 0){
		exit(0);
	}
	else{
		sleep(10);
	}

}
