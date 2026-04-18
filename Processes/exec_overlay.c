// Using exec function
// This program is called by exec_start program

#include <stdio.h>
#include <sys/types.h>
#include "stdlib.h"

int main(int argc, char *argv[])
{
	printf("After the exec my process id is %d\n", getpid());
	printf("My parent process's id is %d\n", getppid());
	
	// Print the first argument
    	if (argc > 1) {
        	printf("Argument received: %s\n", argv[1]);
    	}

    	// Print a custom environment variable
    	char *device = getenv("DEVICE_NAME");
    	if (device != NULL) {
        	printf("Environment Variable (DEVICE_NAME): %s\n", device);
    	} else {
        	printf("DEVICE_NAME not found in environment.\n");
    	}

	printf("\nexec ends\n");
	return 0;
}
