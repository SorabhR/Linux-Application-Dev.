#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	char *file;
	int fd;
	struct flock lock;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		return 1;
	}
	file = argv[1];
	printf("Opening %s\n", file);

	//TODO: Open the File for read write
	fd = open(file,O_RDWR);	
	if (fd == -1) {
        	perror("Error opening file");
        	return 1;
    	}

	printf("\nLocking ... ");
	//TODO: Zero out the 'lock' and initialize the various fields to lock the 
	//Entire file
	//TODO: Invoke fcntl to lock the file
	//struct flock lock;
	memset(&lock,0,sizeof(lock));	
	lock.l_type = F_RDLCK;
	
	//fcntl(fd,F_SETLK,&lock);	
	if (fcntl(fd, F_SETLKW, &lock) == -1) {
            perror("waiting for lock error");
            return 1;
    	}

	printf("Done. Hit <Enter> to unlock ...\n");
	
	getchar();
	printf("Unlocking ... ");
	//TODO: Invoke fcntl to unlock the file
	lock.l_type = F_UNLCK;
	fcntl(fd,F_SETLK,&lock);		
	printf("Done.\n");

	close(fd);
	return 0;
}
