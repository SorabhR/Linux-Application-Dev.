#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>

#define QUEUE_NAME  "/notify_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE];
    unsigned int priority = 10; // Higher priority than default

    // 1. Define queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;           // Max messages in queue
    attr.mq_msgsize = MAX_SIZE;    // Max size of each message
    attr.mq_curmsgs = 0;

    // 2. Open (and create) the queue
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Sender: Enter message to send: ");
    fgets(buffer, MAX_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline

    // 3. Send the message
    if (mq_send(mq, buffer, strlen(buffer) + 1, priority) == -1) {
        perror("mq_send");
    }

    printf("Sender: Message sent with priority %u\n", priority);

    mq_close(mq);
    return 0;
}
