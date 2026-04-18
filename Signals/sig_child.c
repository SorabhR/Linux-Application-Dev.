#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

// The Handler: This runs whenever a child dies
void child_handler(int sig) {
    int status;
    pid_t pid;

    // Use waitpid with WNOHANG to clean up all finished children
    // without blocking the parent if others are still running.
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("\n[Handler] Reaped child %d with status %d\n", pid, status);
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = &child_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; // SA_RESTART prevents system call interruption

    sigaction(SIGCHLD, &sa, NULL);

    if (fork() == 0) { // Child Process
        printf("Child: I'm running for 3 seconds...\n");
        sleep(3);
        printf("Child: Exiting now.\n");
        exit(42);
    }

    // Parent stays busy
    printf("Parent: I'm doing other IoT tasks (PID: %d)...\n", getpid());
    while(1) {
        sleep(1);
        printf("Parent: Working...\n");
    }

    return 0;
}
