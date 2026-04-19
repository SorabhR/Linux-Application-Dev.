#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;

	struct sockaddr_un server_address;
	struct sockaddr_un client_address;

	unlink("server_socket");
	//TODO 1: Get the named fd for the socket
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	//TODO 2: Initialize the sockaddr data structure
	memset(&server_address, 0, sizeof(struct sockaddr_un));
    	server_address.sun_family = AF_UNIX;
    	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);

	//TODO 3: Bind the socket
	bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(struct sockaddr_un));
	//TODO 4: Create the queue for listening 
    	listen(server_sockfd, 1);

	while (1)
	{
		char ch;
		printf("server waiting\n");
		client_len = sizeof(client_address);
		//TODO 5: Accept the connection
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
		//TODO 6: Read the variable from the client, increment and send it back to the client
		read(client_sockfd,(void *)&ch,1);
		printf("Read character is %c \r\n",(char)ch);
		ch++;
		send(client_sockfd,(void *)&ch,1,0);
		break;
	}
	close(client_sockfd);
	close(server_sockfd);
	unlink("server_socket");
	return 0;
}
