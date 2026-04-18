#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "binary_sem.h"

#define SEGMENT_SIZE 0x400

int main()
{
    key_t key = 0x12345; // Using same for both shm & sem
    int segment_id;
    char *shared_memory;
    int sem_id;

    /* Allocate/Access the shared memory segment */
    // Note: IPC_CREAT here allows it to access the existing segment
    segment_id = shmget(key, SEGMENT_SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (segment_id == -1) {
        perror("shmget");
        return 1;
    }
    printf("2: Accessed a shared segment\n");

    /* Attach the shared memory segment */
    shared_memory = (char*)shmat(segment_id, NULL, 0);
    printf("2: Mapped shared memory at address %p\n", shared_memory);

    /* Getting a binary semaphore */
    // TODO 1: Access a binary semaphore created by sem_one
    // Use the 'get' function which doesn't use IPC_CREAT internally
    sem_id = binary_semaphore_get(key, S_IRUSR | S_IWUSR);
    
    if (sem_id < 0)
    {
        perror("sem_get");
        printf("2: Semaphore connect failed. Ensure sem_one is running.\n");
        /* Cleanup */
        shmdt(shared_memory);
        // We don't RMID here because sem_one might still need it
        return 1;
    }
    else
    {
        printf("2: Semaphore connected with id: %d\n", sem_id);
    }

    // TODO 2: Wait call (This will BLOCK if sem_one is still sleeping)
    printf("2: Waiting for semaphore lock...\n");
    binary_semaphore_wait(sem_id);
    printf("2: Wait call returned (Lock Acquired)\n");

    /* Print out the string from shared memory */
    printf("2: Shared memory has \"%s\"\n", shared_memory);

    /* Write a string to the shared memory segment */
    sprintf(shared_memory, "I am fine. How r u?");
    printf("2: Message posted and timepass for a 6 secs\n");
    sleep(6);

    // TODO 3: Post call (Release lock back to sem_one)
    binary_semaphore_post(sem_id);
    printf("2: Post call returned\n");

    // TODO 4: Freeing (Deallocating) the binary semaphore
    // Only one process should deallocate. Usually the last one out.
    binary_semaphore_deallocate(sem_id);
    printf("2: Semaphore deallocated\n");

    shmdt(shared_memory);
    printf("2: Unmapped shared memory\n");
    sleep(2);

    /* Deallocate the shared memory segment */
    shmctl(segment_id, IPC_RMID, 0);
    printf("2: Finally deallocated the shared memory\n");

    return 0;
}
