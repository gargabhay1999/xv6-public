#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    printf(1, "Running tests for nice CLI calls\n");

    // test cases if only 2 argument are provided

    printf(1, "Executing 'nice 3 3' (default value)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "3", "3"};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 3 1' (minimum boundary)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "3", "1"};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 3 5' (maximum boundary)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "3", "5"};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 3 0' (below minimum boundary)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "3", "0"};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 3 6' (above maximum boundary)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "3", "6"};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 3 -1' (negative value)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "3", "-1"};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 3 4' (retesting valid in-range value)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "3", "4"};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    // test cases if only 1 argument is provided

    printf(1, "Executing 'nice 3' (default value)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "3", 0};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 1' (minimum boundary)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "1", 0};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 5' (maximum boundary)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "5", 0};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 0' (below minimum boundary)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "0", 0};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 6' (above maximum boundary)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "6", 0};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice -1' (negative value)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "-1", 0};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();

    printf(1, "Executing 'nice 4' (retesting valid in-range value)\n");
    if (fork() == 0) {
        char *args[] = {"nice", "4", 0};
        exec("nice", args);
        printf(1, "exec failed\n");
        exit();
    }
    wait();


    printf(1, "All tests completed\n");
    exit();
}