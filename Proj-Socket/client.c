#include "stdio.h"
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(){
	int sock_fd;
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));

	sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd == -1){
		perror("Socket creation failed");
		return -1;
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if(connect(sock_fd,&server_addr,sizeof(server_addr)) == -1){
		perror("Connect failed");
		close(sock_fd);
		return -1;
	}
	
	ssize_t sent_bytes;
	char data[10];
	while(1){
		printf("Write the character to send: ");
    		fflush(stdout); // Ensure the prompt actually shows up
		if(fgets(&data,10,stdin) != NULL){
			if(data[0] == '\n')
				continue;
			sent_bytes = send(sock_fd,&data[0],1,0);
			//printf("Character sent\r\n");
			sleep(1);
		}
		//break;
	}
	close(sock_fd);
	return 0;
}
