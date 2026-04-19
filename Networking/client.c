#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	char ch = 'A';

	//TODO 1: Get the fd for the socket
	sockfd = socket(AF_UNIX,SOCK_STREAM,0);
	//TODO 2: Initialize the sockaddr
	memset(&address,0,sizeof(address));
	len = sizeof(address);
	address.sun_family = AF_UNIX;
    	strcpy(address.sun_path, "server_socket");
	
	//TODO 3: Connect to the server 
	result = connect(sockfd,&address,len);
	if(result == -1) {
		perror("oops: client1");
		exit(1);
	}
	//TODO 3: Write a character to socket & read it back
	send(sockfd,&ch,1,0);
	read(sockfd,&ch,1);
	printf("Read character is %c\r\n",ch);
	//TODO 4: Close the socket
	close(sockfd);
	exit(0);
}
