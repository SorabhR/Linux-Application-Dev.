#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

// TODO 1: Obtain a binary semaphore id, allocating if necessary
int binary_semaphore_allocate(key_t key, int sem_flags)
{
    int sem = semget(key,1,sem_flags | IPC_CREAT);
    return sem;
}

// TODO 2: Deallocate a binary semaphore
int binary_semaphore_deallocate(int semid)
{
    semctl(semid, 0, IPC_RMID);
    return 0;
}

// TODO 3: Obtain a pre-existing binary semaphore
int binary_semaphore_get(key_t key, int sem_flags)
{
    int sem = semget(key,1,sem_flags);
    return sem;
}

// TODO 4: Initialize a binary semaphore with a value of 1
int binary_semaphore_set(int semid)
{
    union semun u;
    u.val = 1;
    semctl(semid,0,SETVAL,u);
    return 0;
}

// TODO 5: Wait for the binary semaphore. Decrement the value by -1
int binary_semaphore_wait(int semid)
{
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = -1;      /* Wait/Lock */
    operations[0].sem_flg = SEM_UNDO;
    semop(semid,operations,1);
    /* Use the first (and only) semaphore. */
    /* Decrement by 1. */
    /* Permit undoing. */
    /* Perform the operation with semop*/
    return 0;
}

// TODO 6: Post to a binary semaphore: Increment its value by 1
int binary_semaphore_post(int semid)
{
    struct sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = 1;      /* UnLock */
    operations[0].sem_flg = SEM_UNDO;
    semop(semid,operations,1);

    /* Use the first (and only) semaphore. */
    /* Increment by 1. */
    /* Permit undoing. */
    /* Perform the operation with semop*/
    return 0;
}
