#include <stdio.h>
#include <sys/types.h>

/*Here, we don't need to pass the path name. The execvp() takes the default path*/

int main()
{
	char *temp[2];
	temp[0] = "exec_overlay";
	//temp[1] = "-l";
	temp[1] = NULL;
	char *my_env[] = {"DEVICE_NAME=MySensor", "LOG_LEVEL=DEBUG", NULL };
	
	//execv("/usr/bin/ls", temp);
	int cpid,status;
	cpid = fork();
	if(cpid == 0){
		execve("./exec_overlay", temp,my_env);
		//printf("Just printing %s",LOG_LEVEL);
	}
	else{
		wait(&status);
	}
	printf("This will not print\n");
	return 0;
}
