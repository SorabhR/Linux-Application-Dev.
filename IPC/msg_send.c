#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//TODO 1: Declare the structure with argument of type long and second a character array of say 200
struct my_msg {
    long mtype;       // Message type (Must be > 0)
    char mtext[100];  // The actual data
};

int main(void)
{
	int msgid;
	key_t key;
	int read_cnt;
	char buff[200];

	//TODO 2: Create the message queue with msgget. Use the permissions S_RUSR | S_IWUSR 
	msgid = msgget(123, IPC_CREAT | IPC_EXCL | 0666);

	if (msgid == -1) {
    		if (errno == EEXIST) {
        		printf("Queue already exists. Attaching as a secondary process...\n");
        		// Re-call without IPC_EXCL to just get the ID
        		msgid = msgget(key, 0666);
    		} else {
        		perror("msgget failed");
        		exit(1);
    		}
	} else {
    		printf("Successfully created a NEW unique message queue (ID: %d)\n", msgid);
	}

	printf("Enter lines of text, ^D (on a separate line) to quit:\n");
	
	//TODO 3: Set the message type to 1
	struct my_msg message;
    	message.mtype = 1; // You can use different types for different priority
	while (1)
	{
		//TODO 4: Read into the msgbuf's buffer
		read_cnt = scanf("%[^\n]", buff);
		if (read_cnt == EOF)
		{
			break;
		}
		if (read_cnt == 0) // Null string
		{
			buff[0] = '\0';
		}
		scanf("%*c"); // Read the '\n'
		sprintf(message.mtext, buff);
		//TODO 5: Send the message with msgsnd
		msgsnd(msgid, &message, sizeof(message.mtext), 0);
	}

	msgctl(msgid, IPC_RMID, NULL);

	return 0;
}
