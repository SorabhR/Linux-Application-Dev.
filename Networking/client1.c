#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
        int sockfd;
        int len;
        struct sockaddr_in address;
        int result;
        char ch = 'A';

        // TODO 1: Get the fd for the socket (AF_INET, SOCK_STREAM for TCP)
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Socket creation failed");
            exit(1);
        }

        // TODO 2: Initialize the sockaddr_in
        address.sin_family = AF_INET;
        // Use inet_addr to convert the string "127.0.0.1" to the required binary format
        address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        // Port must match the server's port (9734) and be in Network Byte Order
        address.sin_port = htons(9734); 
        len = sizeof(address);

        // TODO 3: Connect to the server
        result = connect(sockfd, (struct sockaddr *)&address, len);

        if (result == -1) {
                perror("oops: client1 (Check if server is running)");
                exit(1);
        }

        // TODO 4: Write a character to socket & read it back
        printf("Sending character %c to server...\n", ch);
        write(sockfd, &ch, 1);
        
        // Wait for the server's response
        if (read(sockfd, &ch, 1) > 0) {
            printf("Character from server = %c\n", ch);
        } else {
            printf("Server closed connection or error occurred.\n");
        }

        // TODO 5: Close the socket
        close(sockfd);
        exit(0);
}
