#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Required for TODO 4 */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// TODO 1: Obtain a binary semaphore id, allocating if necessary
int binary_semaphore_allocate(key_t key, int sem_flags)
{
    // nsems is 1 for a binary semaphore set
    return semget(key, 1, sem_flags | IPC_CREAT);
}

// TODO 2: Deallocate a binary semaphore.
int binary_semaphore_deallocate(int semid)
{
    // IPC_RMID ignores the second argument (0)
    return semctl(semid, 0, IPC_RMID);
}

// TODO 3: Obtain a pre-existing binary semaphore id without allocating
int binary_semaphore_get(key_t key, int sem_flags)
{
    // Remove IPC_CREAT from flags to ensure we only get an existing one
    return semget(key, 1, sem_flags);
}

// TODO 4: Initialize a binary semaphore with a value of 1
int binary_semaphore_set(int semid)
{
    union semun argument;
    argument.val = 1;
    return semctl(semid, 0, SETVAL, argument);
}

/*
 * Wait on a binary semaphore (P operation).
 * Block until the semaphore value is positive, then decrement it by 1
 */
int binary_semaphore_wait(int semid)
{
    struct sembuf operations[1];

    // TODO 5: Use the first (and only) semaphore
    operations[0].sem_num = 0;
    // TODO 6: Decrement by 1 (Wait/Lock)
    operations[0].sem_op = -1;
    // TODO 7: Permit undoing (Prevent deadlock on crash)
    operations[0].sem_flg = SEM_UNDO;

    // TODO 8: Finally perform the operation using semop
    return semop(semid, operations, 1);
}

/* Post to a binary semaphore (V operation): Increment its value by 1. */
int binary_semaphore_post(int semid)
{
    struct sembuf operations[1];

    // TODO 9: Use the first (and only) semaphore
    operations[0].sem_num = 0;
    // TODO 10: Increment by 1 (Signal/Unlock)
    operations[0].sem_op = 1;
    // TODO 11: Permit undoing
    operations[0].sem_flg = SEM_UNDO;

    // TODO 12: Finally perform the operation using semop
    return semop(semid, operations, 1);
}
