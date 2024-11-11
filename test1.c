#include "types.h"
#include "stat.h"
#include "user.h"

void find_primes(int priority) {
    int i, j, prime;
    int count = 0;
    int pid = getpid();
    for (i = 1; count < 1000; i++) {
        prime = 1;
        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                prime = 0;
                break;
            }
        }
        if (prime) {
            count++;
            printf(1, "PID: %d, Priority: %d, Prime: %d\n", pid, priority, i);
            sleep(100); // Sleep for a small amount to allow context switching
        }
    }
}

int main() {
    int pid3 = fork();
    if (pid3 == 0) {
        nice(getpid(), 1); // Lower priority
        find_primes(1);
        exit();
    }

    int pid2 = fork();
    if (pid2 == 0) {
        nice(getpid(), 5); // Medium priority
        find_primes(5);
        exit();
    }

    int pid1 = fork();
    if (pid1 == 0) {
        nice(getpid(), 5); // Higher priority
        find_primes(5);
        exit();
    }

    wait();
    wait();
    wait();
    exit();
}
