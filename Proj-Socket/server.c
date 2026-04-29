#include "stdio.h"
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define PORT 5000
#define MAX_CLIENTS 1

void myClientHandler(void* args){
	int client_fd = *(int*)args;
	free(args);
	char buffer = 0;
	//memset(buffer,0,256);
	ssize_t read_bytes;
	
	while(1){
		read_bytes = read(client_fd,&buffer,1);
		if(read_bytes > 0){
			printf("The data is %c\r\n",buffer);
		}
		else if(read_bytes == 0){
			printf("Client closed\r\n");
			//pthread_exit(NULL);
			break;
		}else{
			printf("Some error\r\n");
			break;
		}
	}
	close(client_fd);
	return NULL;
}

int main(){
	int sock_fd;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd == -1){
		perror("Socket open failed");
		return -1;
	}
	
	int yes = 1;
	if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(yes)) == -1){
		perror("Socket option set failed");
		return -1;
	}

	struct sockaddr_in myaddr;
	memset(&myaddr,0,sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(PORT);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sock_fd, (const struct sockaddr *)&myaddr, (socklen_t)sizeof(myaddr)) == -1){
		perror("Binding failed");
		return -1;
	}

	if(listen(sock_fd,MAX_CLIENTS) == -1){
		perror("listen failed");
		return -1;
	}


	while(1){
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int* client_fd = malloc(sizeof(int));

		*client_fd = accept(sock_fd,(struct sockaddr *)&client_addr,&client_len);
		pthread_t client_thid = 0;
		
		if(*client_fd == -1){
			perror("Accept failed");
			free(client_fd);
			return -1;
		}
		else{
			printf("Got a client\r\n");
			//start a thread

			if(pthread_create(&client_thid,NULL,myClientHandler,client_fd) == -1){
				perror("client thread creation failed");
				close(*client_fd);
				free(client_fd);
				close(sock_fd);
				return -1;
			}else{
				pthread_detach(client_thid);
			}
		}
	}
	
	//pthread_join(client_thid,NULL);
	//close(client_fd);
	close(sock_fd);

	return 0;
}

