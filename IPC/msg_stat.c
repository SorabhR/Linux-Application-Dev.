#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

int main() {
    // 1. Generate the same key used in your previous program
    //key_t key = ftok("app_config", 65);
    //if (key == -1) {
    //    perror("ftok");
    //    exit(1);
    //}

    // 2. Get the ID of the existing queue
    int msgid = msgget(123, 0666);
    if (msgid == -1) {
        perror("msgget (Queue might not exist)");
        exit(1);
    }

    // 3. Define the structure to hold queue information
    struct msqid_ds buf;

    // 4. Use msgctl with IPC_STAT to fill the buffer
    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl IPC_STAT");
        exit(1);
    }

    // 5. Display the requested information
    printf("--- Message Queue Stats (ID: %d) ---\n", msgid);
    
    // Total messages currently waiting in the queue
    printf("Current message count:      %lu\n", buf.msg_qnum);
    
    // Maximum bytes allowed in the queue (System limit for this queue)
    printf("Max bytes allowed in Queue: %lu bytes\n", buf.msg_qbytes);
    
    // PID of the last process that performed a msgsnd()
    printf("Last sending process PID:   %d\n", buf.msg_lspid);
    
    // PID of the last process that performed a msgrcv()
    printf("Last receiving process PID: %d\n", buf.msg_lrpid);

    return 0;
}
