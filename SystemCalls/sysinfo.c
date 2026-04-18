#include <linux/kernel.h>
//#include <linux/sys.h>
#include <stdio.h>
#include <sys/sysinfo.h>


int main() {
	const long minute = 60;
	const long hour = minute *60;
	const long day = hour * 24;
	const double megabyte = 1024 * 1024;
	struct sysinfo info;
	
	if (sysinfo(&info) != 0) {
        	perror("sysinfo error");
        	return 1;
    	}

    	// Convert everything to Megabytes using mem_unit
    	unsigned long total_mb = (info.totalram * info.mem_unit) / 1024 / 1024;
    	unsigned long free_mb = (info.freeram * info.mem_unit) / 1024 / 1024;

    	printf("Uptime: %ld days, %ld hours\n", 
            info.uptime / 86400, (info.uptime % 86400) / 3600);
    	printf("Total RAM: %lu MB\n", total_mb);
    	printf("Free RAM:  %lu MB\n", free_mb);
    	printf("Process count: %d\n", info.procs);	
	//TODO 1: Get the system info using sysinfo system call

	//TODO 2: Print the total up time
	//TODO 3: Print the total RAM in MB
	//TODO 4: Print the Free Ram 
	//TODO 5: Print the process count 
	return 0;
}
