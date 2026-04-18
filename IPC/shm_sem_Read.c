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

    int shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    char *ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    // Open existing semaphore
    sem_t *sem = sem_open(sem_name, 0); 

    printf("Reader: Waiting for lock...\n");
    sem_wait(sem); // Will block here if Writer is in Critical Section
    
    printf("Reader: Lock acquired! Content: %s\n", ptr);
    
    sem_post(sem); // Release lock

    // Cleanup
    munmap(ptr, SIZE);
    close(shm_fd);
    sem_close(sem);
    
    // Final cleanup (unlink)
    shm_unlink(shm_name);
    sem_unlink(sem_name);
    return 0;
}
