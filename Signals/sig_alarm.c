#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void parent_handler(int sig) {
    printf("Parent: Received SIGALRM from child. I am still alive!\n");
}

// Note the 3-argument signature required for SA_SIGINFO
void advanced_handler(int sig, siginfo_t *info, void *context) {
    // Access the PID from the info pointer
    pid_t sender_pid = info->si_pid;

    // Use write() for thread-safety in a handler
    printf("\n[Signal Received] %d from Process PID: %d\n", sig, sender_pid);

    if (sender_pid == 0) {
        printf("Note: PID 0 usually means the signal was sent by the Kernel (e.g., timer).\n");
    }
}

int main() {
    // 1. Setup the handler IN THE PARENT first
    struct sigaction sa;
    //sa.sa_handler = parent_handler;
    sigemptyset(&sa.sa_mask);
    //sa.sa_flags = 0;
    
    // 2. Assign the 3-argument function to sa_sigaction (NOT sa_handler)
    sa.sa_sigaction = advanced_handler;

    // 3. CRITICAL: Set the SA_SIGINFO flag
    sa.sa_flags = SA_SIGINFO;
    
    sigaction(SIGALRM, &sa, NULL);

    pid_t pid = fork();

    if (pid == 0) { // Child
        sleep(1); 
        printf("Child: Sending SIGALRM to parent (PID: %d) from child (PID: %d)\n", getppid(),getpid());
        kill(getppid(), SIGALRM);
        exit(0);
    } else { // Parent
        printf("Parent: Waiting for signal...\n");
        pause(); // Wait until a signal arrives
        printf("Parent: Continuing execution after signal.\n");
    }

    return 0;
}
