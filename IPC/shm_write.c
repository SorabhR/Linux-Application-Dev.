#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    const char *name = "/iot_shm";  // Name must start with /
    const int SIZE = 4096;          // 4KB
    const char *message = "Data from LoRaWAN Sensor Node 1";

    int shm_fd;
    void *ptr;

    // 1. Create the shared memory object
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // 2. Configure the size of the shared memory segment
    ftruncate(shm_fd, SIZE);

    // 3. Memory map the shared memory object into the process address space
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // 4. Write to the shared memory
    sprintf(ptr, "%s", message);
    printf("Writer: Written to memory: %s\n", (char *)ptr);

    // Note: We don't unlink here because the reader needs to access it.
    return 0;
}
