#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    const char *name = "/iot_shm";
    const int SIZE = 4096;

    int shm_fd;
    void *ptr;

    // 1. Open the existing shared memory object
    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // 2. Memory map the shared memory object
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // 3. Read from the shared memory
    printf("Reader: Read from memory: %s\n", (char *)ptr);

    // 4. Cleanup: Unmap and Unlink (Remove)
    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(name);

    return 0;
}
