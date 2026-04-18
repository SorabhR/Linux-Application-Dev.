#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>

#define QUEUE_NAME  "/iot_gateway_queue"
#define MAX_SIZE    1024

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE + 1];
    unsigned int priority;
    ssize_t bytes_read;

    // 1. Open the existing queue for reading
    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Receiver: Waiting for message...\n");

    // 2. Receive the message
    // Note: buffer size must be >= mq_msgsize attribute
    bytes_read = mq_receive(mq, buffer, MAX_SIZE, &priority);
    
    if (bytes_read >= 0) {
        buffer[bytes_read] = '\0';
        printf("Receiver: Received: \"%s\"\n", buffer);
        printf("Receiver: Message Priority: %u\n", priority);
    } else {
        perror("mq_receive");
    }

    // 3. Cleanup
    mq_close(mq);
    mq_unlink(QUEUE_NAME); // Remove the queue from the system

    return 0;
}
