#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include "sig.h"
#include "em_registers.h"
#include "sock.h"

#define FIFO_NAME "myfifo"
#define PORT_NUM 2000
int main(int argc,char* argv[])
{
   char* ip_addr;
   int sock_fd;
    if (argc != 2)
    {
        printf("Usage: %s <ip addr>\n", argv[0]);
        return 1;
    }

    ip_addr = argv[1];
    
    
    sock_fd = socket(AF_INET,SOCK_STREAM,0); 
    if(sock_fd < 0){
	  perror("Socket open failed");
	 return -1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_NUM);

    // 2. Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if(connect(sock_fd,(struct sockaddr_t*)&serv_addr,sizeof(serv_addr)) < 0){
	    perror("Client connection failed");
	    return -1;
    }
     

    //int fd;
    struct em_registers reg;

    /*
    // TODO 1: Create the FIFO
    if(mkfifo(FIFO_NAME,0666) == -1){
	    if(errno != EEXIST){
		    perror("FIFO creation failed");
		    return -1;
	    }
    }*/
    printf("Waiting for writers ...\n");
    // TODO 2: Open the FIFO
    /*fd = open(FIFO_NAME,O_RDONLY);
    if(fd == -1){
	    perror("FIFO open failed");
	    return -1;
    }*/
    printf("Got a writer:\n");

	//TODO 3: Keep on reading the EM Registers (struct em_registers) from FIFO
    /*while (read(fd,&reg,sizeof(reg)) > 0)     
    {
        printf("Va = %u, Vb = %u Vc = %u\n", reg.va, reg.vb, reg.vc);
        printf("Va Max = %u, Vb Max = %u Vc Max = %u\n", reg.va_max, reg.vb_max, reg.vc_max);
    }*/
    //while(1);
    while (read_eth(sock_fd,&reg,sizeof(reg)) > 0)     
    {
        printf("Va = %u, Vb = %u Vc = %u\n", reg.va, reg.vb, reg.vc);
        printf("Va Max = %u, Vb Max = %u Vc Max = %u\n", reg.va_max, reg.vb_max, reg.vc_max);
    }
   
    //close(fd);
    close(sock_fd);
    return 0;
}
