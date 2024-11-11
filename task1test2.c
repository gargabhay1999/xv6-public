


// printf(1, "Running test1.c tests for nice CLI calls\n");

//     // Test 1: Set a priority out of range (9) for current process
//     printf(1, "nice 9\n");
//     if (fork() == 0) {
//         char *args[] = {"nice", "9", 0};
//         exec("nice", args);
//         exit();
//     }
//     wait();

#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    printf(1, "Running test1.c tests for nice CLI calls\n");

    // Test 1: Set a priority out of range (9) for current process
    printf(1, "nice 4\n");

    // Fork a child process to run the 'nice' command with argument 9
    if (fork() == 0) {
        // Prepare arguments to pass to exec() for the 'nice' command
        char *args[] = {"nice", "4", 0};
        
        // Execute the 'nice' command with the argument 9
        exec("nice", args);
        
        // If exec() fails, exit the child process
        printf(1, "exec failed\n");
        exit();
    }

    // Wait for the child process to finish
    wait();

    // Optionally, print a message after test completion
    printf(1, "Test completed\n");

    exit();
}
