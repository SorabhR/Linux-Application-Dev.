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
    int server_sockfd, client_sockfd;
    socklen_t server_len, client_len; // Use socklen_t for socket lengths
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    // TODO 1: Get the named fd for the socket (AF_INET for IPv4, SOCK_STREAM for TCP)
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // TODO 2: Initialize the sockaddr data structure
    server_address.sin_family = AF_INET;
    // INADDR_ANY binds to all available interfaces (WiFi, Ethernet, etc.)
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
    // Use htons (host-to-network-short) to ensure the port is Big Endian
    server_address.sin_port = htons(9734); 
    server_len = sizeof(server_address);

    // TODO 3: Bind the socket
    if (bind(server_sockfd, (struct sockaddr *)&server_address, server_len) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // TODO 4: Create the queue for listening (backlog of 5)
    listen(server_sockfd, 5);

    while (1)
    {
        char ch;
        printf("server waiting on port 9734...\n");
        client_len = sizeof(client_address);

        // TODO 5: Accept the connection (Pass client_len by reference!)
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        
        if (client_sockfd < 0) {
            perror("Accept failed");
            continue;
        }

        // TODO 6: Read from client, increment and send back
        if (read(client_sockfd, &ch, 1) > 0) {
            printf("Read character from %s is %c\n", inet_ntoa(client_address.sin_addr), ch);
            ch++;
            write(client_sockfd, &ch, 1);
        }

        close(client_sockfd); // Close the connection with this specific client
    }

    close(server_sockfd);
    return 0;
}
