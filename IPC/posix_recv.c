#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <signal.h>
#include <unistd.h>

#define QUEUE_NAME  "/my_queue"
#define MAX_SIZE    1024

mqd_t mq;

void msg_handler(int signum){
    ssize_t bytes_read;
    char buffer[MAX_SIZE + 1];
    unsigned int priority;
    bytes_read = mq_receive(mq, buffer, MAX_SIZE, &priority);
    
    if (bytes_read >= 0) {
        buffer[bytes_read] = '\0';
        printf("Receiver: Received: \"%s\"\n", buffer);
        printf("Receiver: Message Priority: %u\n", priority);
    } else {
        perror("mq_receive");
    }

}

int main() {
    ssize_t bytes_read;

    // 1. Open the existing queue for reading
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Receiver: Waiting for message...\n");
    
    struct sigaction sa;
    sa.sa_handler = msg_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1,&sa,NULL) == -1){
	    perror("Handler failed");
	    return -1;
    }

    // 2. Receive the message
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    mq_notify(mq,&sev);
    // Note: buffer size must be >= mq_msgsize attribute
    while(1){
	    pause();
    }
    // 3. Cleanup
    mq_close(mq);
    mq_unlink(QUEUE_NAME); // Remove the queue from the system

    return 0;
}
