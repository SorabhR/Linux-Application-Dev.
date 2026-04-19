#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pthread_attr_t attr;
    size_t stack_size;
    pthread_t tid;

    // 1. Initialize the attribute object
    pthread_attr_init(&attr);

    // 2. Get and print the current (default) stack size
    pthread_attr_getstacksize(&attr, &stack_size);
    printf("Default Stack Size: %ld bytes (%.2f MB)\n", 
            stack_size, (double)stack_size / (1024 * 1024));

    // 3. Set the desired stack size (e.g., 2 MB)
    // Note: Most systems require stack size to be a multiple of the page size
    size_t desired_size = 2 * 1024 * 1024; 
    pthread_attr_setstacksize(&attr, desired_size);

    // 4. Verify the change
    pthread_attr_getstacksize(&attr, &stack_size);
    printf("Updated Stack Size: %ld bytes (%.2f MB)\n", 
            stack_size, (double)stack_size / (1024 * 1024));

    // 5. Cleanup
    pthread_attr_destroy(&attr);

    return 0;
}
