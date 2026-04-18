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

    /* Allocate a shared memory segment */
    segment_id = shmget(key, SEGMENT_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (segment_id == -1) {
        perror("shmget");
        return 1;
    }
    printf("1: Allocated a shared segment\n");

    /* Attach the shared memory segment */
    shared_memory = (char*)shmat(segment_id, NULL, 0);
    printf("1: Mapped shared memory at address %p\n", shared_memory);

    // TODO 1: Allocate a binary semaphore
    sem_id = binary_semaphore_allocate(key, S_IRUSR | S_IWUSR);
    printf("1: Semaphore created with id: %d\n", sem_id);

    // TODO 2: Initialize the semaphore to 1 (Mutex unlocked)
    binary_semaphore_set(sem_id);
    printf("1: Semaphore set to 1\n");

    // TODO 3: Wait call (Lock the resource)
    binary_semaphore_wait(sem_id);
    printf("1: Wait call returned (Locked)\n");

    sprintf(shared_memory, "Hi how r you?");
    printf("1: Message posted and timepass for half a minute. Now run ./sem_two\n");
    
    // While sleeping, this process holds the lock. 
    // ./sem_two will block at its wait call until this sleep ends and post is called.
    sleep(30);

    // TODO 4: Post call (Unlock so sem_two can read/write)
    binary_semaphore_post(sem_id);
    printf("1: Post call returned (Unlocked)\n");

    // We wait again to see what sem_two wrote back
    printf("1: Waiting for sem_two to respond...\n");
    
    // TODO 5: Wait call
    binary_semaphore_wait(sem_id);
    printf("1: Wait call returned\n");

    /* Print out the string from shared memory */
    printf("1: Shared memory has \"%s\"\n", shared_memory);

    // TODO 6: Post Call
    binary_semaphore_post(sem_id);
    printf("1: Post call returned\n");

    /* Detach the shared memory segment */
    shmdt(shared_memory);
    printf("1: Unmapped shared memory\n");

    return 0;
}
