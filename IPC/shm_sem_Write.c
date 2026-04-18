#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

int main() {
    const char *shm_name = "/iot_shm";
    const char *sem_name = "/iot_sem";
    const int SIZE = 4096;

    // 1. Create/Open Shared Memory
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    char *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // 2. Create/Open Named Semaphore
    // Initial value is 1 (Unlocked)
    sem_t *sem = sem_open(sem_name, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) { perror("sem_open"); exit(1); }

    printf("Writer: Locking semaphore...\n");
    sem_wait(sem); // P operation (Decrement)
    
    // --- Critical Section Start ---
    printf("Writer: Entering Critical Section. Writing data...\n");
    sprintf(ptr, "LoRa Packet ID: %d, Payload: Temperature=24C", rand() % 100);
    sleep(5); // Simulate processing time
    // --- Critical Section End ---

    printf("Writer: Unlocking and exiting...\n");
    sem_post(sem); // V operation (Increment)

    munmap(ptr, SIZE);
    close(shm_fd);
    sem_close(sem);
    return 0;
}
