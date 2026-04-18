#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

// Helper to lock (Wait)
void sem_lock(int semid) {
    struct sembuf sb = {0, -1, SEM_UNDO};
    semop(semid, &sb, 1);
}

// Helper to unlock (Signal)
void sem_unlock(int semid) {
    struct sembuf sb = {0, 1, SEM_UNDO};
    semop(semid, &sb, 1);
}

int main()
{
	int semid, pid;
	struct sembuf sop;
	
	//TODO 1: Allocate the semaphore with semget
	semid = semget(123, 1, 0666 | IPC_CREAT);
	//TODO 2: Set the semaphore value to 1 with semctl
	//union semun arg;
   	int val = 1;
    	semctl(semid, 0, SETVAL, val);
	
	pid = fork();
	if (pid == 0)
	{
		printf("Child before semop\n");
		//TODO 3: Populate the sembuf to decrement the first semaphore
		sem_lock(semid);
		//Set sem_flg to 0
		//TODO 4: Finally invoke the semop
		printf("Child in critical section for 10 seconds\n");
		printf("Parent may have to wait\n");
		sleep(10);
		printf("Child coming out of critical section\n");
		sem_unlock(semid);
		//TODO 5: Populate the sembuf to increment the first semaphore
		//Set sem_flg to 0
		//TODO 6: Finally invoke the semop
	}
	else
	{
		int child_status;

		printf("Parent before semop\n");
		sem_lock(semid);
		//TODO 7: Populate the sembuf to decrement the first semaphore
		//Set sem_flg to 0
		//TODO 8: Finally invoke the semop
		printf("Parent in critical section\n");
		printf("Child may have to wait\n");
		sleep(10);
		printf("Parent coming out of critical section\n");
		sem_unlock(semid);
		//TODO 9: Populate the sembuf to increment the first semaphore
		//Set sem_flg to 0
		//TODO 10: Finally invoke the semop
	}

	return 0;
}
