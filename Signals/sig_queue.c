#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// 1. The handler for Real-Time signals
void rt_handler(int sig, siginfo_t *info, void *context) {
    // info->si_value.sival_int retrieves the data sent with the signal
    printf("Handler: Received signal %d with value: %d\n", sig, info->si_value.sival_int);
}

int main() {
    struct sigaction sa;
    sigset_t block_mask;

    // 2. Setup the handler with SA_SIGINFO
    sa.sa_sigaction = rt_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO; 

    // We use SIGRTMIN (the first available real-time signal)
    sigaction(SIGRTMIN, &sa, NULL);

    // 3. BLOCK the signal first so we can "build up" a queue
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGRTMIN);
    sigprocmask(SIG_BLOCK, &block_mask, NULL);

    printf("PID: %d. Blocking SIGRTMIN and queuing 3 signals...\n", getpid());

    // 4. Queue three signals with different values
    union sigval value;
    for (int i = 1; i <= 3; i++) {
        value.sival_int = i * 100; // Sending 100, 200, 300
        printf("Queuing signal with value %d...\n", value.sival_int);
        sigqueue(getpid(), SIGRTMIN, value);
    }

    printf("Waiting 2 seconds before unblocking...\n");
    sleep(2);

    // 5. UNBLOCK - All 3 signals will now fire in order
    printf("Unblocking now!\n");
    sigprocmask(SIG_UNBLOCK, &block_mask, NULL);

    // Give the handler time to run 3 times
    sleep(1); 
    return 0;
}
