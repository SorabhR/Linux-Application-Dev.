#define _GNU_SOURCE  /* Required for F_GETPIPE_SZ and F_SETPIPE_SZ */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int pipefd[2];
    int current_size, new_size;

    // 1. Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // 2. Get the existing pipe size
    current_size = fcntl(pipefd[0], F_GETPIPE_SZ);
    if (current_size < 0) {
        perror("F_GETPIPE_SZ");
    } else {
        printf("Current pipe size: %d bytes (%d KB)\n", current_size, current_size / 1024);
    }

    // 3. Modify the pipe size (Setting to 128 KB)
    new_size = 128 * 1024; 
    if (fcntl(pipefd[0], F_SETPIPE_SZ, new_size) < 0) {
        perror("F_SETPIPE_SZ");
    } else {
        // Verify the change
        current_size = fcntl(pipefd[0], F_GETPIPE_SZ);
        printf("New pipe size:     %d bytes (%d KB)\n", current_size, current_size / 1024);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}
