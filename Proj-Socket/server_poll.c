#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5000
#define MAX_FDS 10 // Let's monitor up to 10 things

int main() {
    int listen_fd, new_fd;
    struct sockaddr_in addr;
    struct pollfd fds[MAX_FDS];
    int nfds = 1; // Start with 1 (the listening socket)

    // 1. Standard Setup
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(listen_fd, 5);

    // 2. Initialize the poll array
    memset(fds, 0, sizeof(fds));
    fds[0].fd = listen_fd;
    fds[0].events = POLLIN; // Monitor for "Incoming Data/Connection"

    printf("Server polling on port %d...\n", PORT);

    while (1) {
        // 3. Wait for something to happen (timeout = -1 means wait forever)
        int ret = poll(fds, nfds, -1);
        if (ret < 0) { perror("poll"); break; }

        // 4. Loop through our fds to find the active one
        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                
                // CASE A: New connection on the listening socket
                if (fds[i].fd == listen_fd) {
                    new_fd = accept(listen_fd, NULL, NULL);
                    if (nfds < MAX_FDS) {
                        fds[nfds].fd = new_fd;
                        fds[nfds].events = POLLIN;
                        nfds++;
                        printf("New client connected (Socket %d)\n", new_fd);
                    } else {
                        printf("Server full, closing connection\n");
                        close(new_fd);
                    }
                } 
                // CASE B: Existing client sent data
                else {
                    char buf;
                    int bytes = read(fds[i].fd, &buf, 1);
                    if (bytes <= 0) {
                        printf("Socket %d closed\n", fds[i].fd);
                        close(fds[i].fd);
                        // Remove from array (swap with last element)
                        fds[i] = fds[nfds - 1];
                        nfds--;
                        i--; // Re-check this index
                    } else {
                        printf("Socket %d sent: %c\n", fds[i].fd, buf);
                    }
                }
            }
        }
    }
    close(listen_fd);
    return 0;
}
