#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int x = 10;
    pid_t pid = vfork();

    if (pid == 0) { // Child
        printf("Child: changing x to 20\n");
        x = 20; // This changes the PARENT'S x too!
        
        // Child MUST call exec or _exit
        char *args[] = {"/bin/ls", NULL};
        execv(args[0], args);
        
        // If exec fails, must use _exit, not exit
        _exit(1); 
    } else { // Parent
        // Parent was suspended until the child called execv
        printf("Parent: x is now %d\n", x); // Output: x is now 20
    }

    return 0;
}
