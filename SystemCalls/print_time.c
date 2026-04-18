#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main() {
	struct timeval tv;
	struct tm* ptm;
	char time_str[40];
	long milliseconds;

	//TODO 1: Populate the timeval structure with gettimeofday
	//TODO 2: Use localtime to get struct tm
	//TODO 3: Use strftime to get the time in desired format
	// 1. Get raw system time (System Call)
    	gettimeofday(&tv, NULL);

    	// 2. Convert to local time structure (Library Call)
    	ptm = localtime(&tv.tv_sec);

    	// 3. Format into a readable string (Library Call)
    	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ptm);

	milliseconds = tv.tv_usec / 1000;
	printf("%s.%03ld\n", time_str, milliseconds);
	return 0;
}
