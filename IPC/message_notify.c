#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define QUEUE_NAME "/notify_queue"
#define MAX_SIZE   1024

mqd_t mq;
struct sigevent sev;

// The improved handler using the sigaction signature
void message_arrival_handler(int signum, siginfo_t *info, void *context) {
    char buffer[MAX_SIZE + 1];
    unsigned int priority;

    // 1. Re-register for the next notification (Still required for mq_notify)
    mq_notify(mq, &sev);

    // 2. The "Drain Loop": Read ALL messages currently in the queue
    while (1) {
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, &priority);
        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0';
            printf("\n[Async sigaction] Received: %s\n", buffer);
        } else {
            if (errno == EAGAIN) break; // Queue is now empty
            perror("mq_receive");
            break;
        }
    }
}

int main() {
    struct mq_attr attr;
    struct sigaction sa;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;

    // Open with O_NONBLOCK for the drain loop
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attr);

    // 3. Configure sigaction
    sa.sa_flags = SA_SIGINFO;            // Use the extended signal handler signature
    sa.sa_sigaction = message_arrival_handler;
    sigemptyset(&sa.sa_mask);            // Block nothing by default...
    sigaddset(&sa.sa_mask, SIGUSR1);     // ...except other SIGUSR1s while we work
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // 4. Register for notification
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    mq_notify(mq, &sev);

    printf("Waiting for messages using sigaction. Doing work...\n");

    while (1) {
        pause(); // Wait for any signal to save CPU
    }

    return 0;
}
