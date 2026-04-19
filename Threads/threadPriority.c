#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main() {
    pthread_attr_t attr;
    int policy;
    struct sched_param param;

    // 1. Initialize attributes
    pthread_attr_init(&attr);

    // 2. Get and print current scheduling policy
    pthread_attr_getschedpolicy(&attr, &policy);
    printf("Default Policy: %s\n", 
           (policy == SCHED_OTHER) ? "SCHED_OTHER (Normal)" : "Other");

    // 3. Set policy to SCHED_FIFO
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    // 4. Set priority (1 to 99 for Real-Time)
    param.sched_priority = 10;
    pthread_attr_setschedparam(&attr, &param);

    // 5. Explicitly tell the system to use the attributes we just set
    // This ensures it doesn't inherit the parent's (main) settings
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    // 6. Verify the changes
    pthread_attr_getschedpolicy(&attr, &policy);
    pthread_attr_getschedparam(&attr, &param);

    printf("New Policy: %s\n", (policy == SCHED_FIFO) ? "SCHED_FIFO (Real-Time)" : "Fail");
    printf("New Priority: %d\n", param.sched_priority);

    // 7. Cleanup
    pthread_attr_destroy(&attr);

    return 0;
}
