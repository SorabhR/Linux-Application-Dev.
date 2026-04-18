#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void writer(const char *message, int count, int fd)
{
    int len = strlen(message);
    for(; count > 0; --count)
    {
        if (write(fd, message, len) < 0)
        {
            printf("Unable to write\n");
            return;
        }
        sleep(2);
    }
}

void reader(int fd,int counter)
{
    char buffer[1024];
	int i = 0;
    for(;counter>0;counter--){
    	if (read(fd, buffer, sizeof(buffer)) > 0)
        	printf("%s %d\n", buffer, i++);
    }
}

int main()
{
    int fds[2];
    int fds_bi[2];
    pid_t pid;
    
    // TODO 1: Create the pipe
	if(pipe(fds) == -1){
		perror("Pipe creation failed");
		return 1;
	}
	if(pipe(fds_bi) == -1)
		perror("Second pip failed");
    // TODO 2: Create the new process
	pid = fork();
    // TODO 3: Make a child read from the pipe
	if (pid == (pid_t)0)
	{
		close(fds[1]);
		close(fds_bi[0]);
		printf("I am in child\n");
		reader(fds[0],5);
		printf("Now child will send to parent\r\n");
		writer("Hello, World", 5, fds_bi[1]);
	}
    else // TODO 4: Make a parent to write to pipe
	{
		close(fds[0]);
		close(fds_bi[1]);
		printf("I am in parent\n");
		sleep(5);
		writer("Hello, World", 5, fds[1]);
		reader(fds_bi[0],5);
	}
		
    return 0;
}
