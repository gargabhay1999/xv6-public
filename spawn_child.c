
#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    // Number of child processes to spawn
    const int numChildren = 5;

    // Spawn child processes
    for (int i = 0; i < numChildren; i++) {
        if (fork() == 0) { // Child process
            printf(1, "pid: %d\n", getpid());
            exec(argv[1], &argv[1]); // Execute the specified program
            printf(1, "Failed to execute %s\n", argv[1]);
            exit();
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < numChildren; i++) {
        wait();
    }

    exit();
}