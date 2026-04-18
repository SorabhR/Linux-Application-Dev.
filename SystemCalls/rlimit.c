#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

int main() {

	struct rlimit rl;

	//TODO 1: Get the current cpu limit with getrlimit
	// 1. Get the current limit for open files
    	if (getrlimit(RLIMIT_CPU, &rl) == 0) {
        	printf("Current limits -> Soft: %ld, Hard: %ld\n", 
                	(long)rl.rlim_cur, (long)rl.rlim_max);
    	}
	printf("Current limit is %u\n", rl.rlim_cur);
	//TODO 2: set the current limit to 1 sec with setrlimit
	rl.rlim_cur = 1;
	setrlimit(RLIMIT_CPU,&rl);
	while (1);
	return 0;
}

