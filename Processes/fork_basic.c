#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t child_pid;

	printf("The main program process id is %d\n", (int)getpid());
	//TODO 1: Invoke the fork and get the return value in child_pid
	child_pid = fork();
	//TODO 2: Display the parent, self and child process id in parent process
	if(child_pid != 0){
		printf("This is parent and parent id,self id,child id are %d,%d,%d\n",(int)getppid(),(int)getpid(),(int)			child_pid);
		sleep(1);
	}
	//TODO 3: Display the self and parent process id in the child process
	else
		printf("This is child and parentid and self id are %d,%d\n",(int)getppid(),(int)getpid());
	return 0;
}
