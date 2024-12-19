#include "types.h"
#include "stat.h"
#include "user.h"

void leak_memory(int iterations) {
    for(int i = 0; i < iterations; i++) {
        malloc(1024);  // Allocate 1KB
        // Never free the memory
        if (i % 100 == 0) {
            printf(1, "Iteration %d: Allocated 1KB\n", i);
            sleep(1);  // Add some delay to make it easier to observe
        }
        
    }
}

int main(void) {
    printf(1, "Starting memory leak program...\n");
    leak_memory(500);
    printf(1, "Program finished\n");
    exit();
}