#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "fifo"

int main(void)
{
	FILE *fp;
	char readbuf[80];

	//TODO 1: Create the FIFO using library function
	if(mkfifo("/tmp/myfifo",0666) < 0)
		perror("Error creating fifo file");

	printf("\nWaiting for writer clients ...\n");
	//TODO 2: Open the FIFO file
   	fp = fopen("/tmp/myfifo","r");
	//TODO 3: Read & print until EOF
	while(fgets(readbuf,10,fp) != NULL)
		printf("%s\r\n",readbuf);

	fclose(fp);
	unlink("/tmp/myfifo");

	return 0;
}
