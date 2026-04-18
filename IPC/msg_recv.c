#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//TODO 1: Declare the structure with argument of type long and second a character array of say 200
struct my_msg {
    long mtype;
    char mtext[100];
};

int main(void)
{
	int msgid;
	key_t key;

	//TODO 2: Access the Message queue created by msg_send process. Use the permissions S_RUSR | S_IWUSR 
	msgid = msgget(123, 0666);
	printf("Ready to receive messages\n");
	
	struct my_msg message;
	for (;;) /* it  never quits! */
	{
		//TODO 3: Receive the message with msgrcv and display the same
		msgrcv(msgid, &message, sizeof(message.mtext), 1, 0);

    		printf("Data Received: %s\n", message.mtext);

	}
	msgctl(msgid, IPC_RMID, NULL);

	return 0;
}
