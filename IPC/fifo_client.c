#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define FIFO_FILE "/tmp/myFifo"

// Handler for when the reader disappears
void broken_pipe(int sno)
{
    printf("\n[Signal %d] Reader has disconnected! Closing down.\n", sno);
    exit(1); 
}

int main(int argc, char *argv[])
{
    int cnt = 1, i;
    FILE *fp;
    struct sigaction sa;

    if ((argc != 2) && (argc != 3))
    {
        printf("Usage: ./fifoclient <string> [ <cnt> ]\n");
        return 1;
    }

    if (argc == 3)
    {
        cnt = atoi(argv[2]);
    }

    // Register SIGPIPE handler to prevent crash if reader closes early
    sa.sa_handler = broken_pipe;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGPIPE, &sa, NULL);

    // Create the FIFO
    // 0666 gives read/write permissions to everyone
    if (mkfifo(FIFO_FILE, 0666) < 0)
    {
        if (errno != EEXIST) { // Ignore error if FIFO already exists
            perror("mkfifo error");
            return 1;
        }
    }

    printf("Waiting for a reader to connect to %s...\n", FIFO_FILE);

    // Open the FIFO in Write mode
    // NOTE: This blocks until someone runs 'cat /tmp/myFifo' or similar
    fp = fopen(FIFO_FILE, "w");
    if (fp == NULL)
    {
        perror("fopen error");
        return 1;
    }

    printf("Sending %d messages: ", cnt);
    fflush(stdout);

    for (i = 0; i < cnt; i++)
    {
        // Use fprintf for formatted output (great for adding newlines)
        fprintf(fp, "%s (Message #%d)\n", argv[1], i + 1);
        
        // Critical: Force data out of the RAM buffer into the FIFO
        fflush(fp);

        printf(".");
        fflush(stdout);
        sleep(1);
    }

    printf(" done\n");
    fclose(fp);

    return 0;
}
