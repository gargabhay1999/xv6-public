#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    int pid = getpid();
    int new_nice_val, old_nice;
    int passed_count = 0;
    int failed_count = 0;

    printf(1, "Starting comprehensive tests for nice system call\n");

    // Test 1: Set to default value (3)
    printf(1, "Test 1: Setting nice value to default (3)\n");
    new_nice_val = 3;
    old_nice = nice(pid, new_nice_val);
    if (old_nice == -1) {
        failed_count ++;
        printf(1, "Failed: can't set nice value to %d for pid %d: out of bounds or invalid PID\n", new_nice_val, pid);
    } else {
        passed_count ++;
        printf(1, "Success: pid %d, old_nice_value: %d, new_nice_value: %d\n", pid, old_nice, new_nice_val);
    }
    printf(1,"\n");

    // Test 2: Set to the minimum boundary (1)
    printf(1, "Test 2: Setting nice value to minimum boundary (1)\n");
    new_nice_val = 1;
    old_nice = nice(pid, new_nice_val);
    if (old_nice == -1) {
        failed_count ++;
        printf(1, "Failed: can't set nice value to %d for pid %d: out of bounds or invalid PID\n", new_nice_val, pid);
    } else {
        passed_count ++;
        printf(1, "Passed: pid %d, old_nice_value: %d, new_nice_value: %d\n", pid, old_nice, new_nice_val);
    }
    printf(1,"\n");

    // Test 3: Set to the maximum boundary (5)
    printf(1, "Test 3: Setting nice value to maximum boundary (5)\n");
    new_nice_val = 5;
    old_nice = nice(pid, new_nice_val);
    if (old_nice == -1) {
        failed_count ++;
        printf(1, "Failed: can't set nice value to %d for pid %d: out of bounds or invalid PID\n", new_nice_val, pid);
    } else {
        passed_count ++;
        printf(1, "Passed: pid %d, old_nice_value: %d, new_nice_value: %d\n", pid, old_nice, new_nice_val);
    }
    printf(1,"\n");

    // Test 4: Try to set below the minimum boundary (e.g., 0)
    printf(1, "Test 4: Attempting to set nice value below minimum (0)\n");
    new_nice_val = 0;
    old_nice = nice(pid, new_nice_val);
    if (old_nice == -1) {
        passed_count ++;
        printf(1, "Passed: can't set nice value to %d for pid %d: out of bounds or invalid PID\n", new_nice_val, pid);
    } else {
        failed_count ++;
        printf(1, "Failed: pid %d, old_nice_value: %d, new_nice_value: %d\n", pid, old_nice, new_nice_val);
    }
    printf(1,"\n");

    // Test 5: Try to set above the maximum boundary (e.g., 6)
    printf(1, "Test 5: Attempting to set nice value above maximum (6)\n");
    new_nice_val = 6;
    old_nice = nice(pid, new_nice_val);
    if (old_nice == -1) {
        passed_count ++;
        printf(1, "Passed: can't set nice value to %d for pid %d: out of bounds or invalid PID\n", new_nice_val, pid);
    } else {
        failed_count ++;
        printf(1, "Failed: pid %d, old_nice_value: %d, new_nice_value: %d\n", pid, old_nice, new_nice_val);
    }
    printf(1,"\n");

    // Test 6: Try to set to a negative value (e.g., -1)
    printf(1, "Test 6: Attempting to set a negative nice value (-1)\n");
    new_nice_val = -1;
    old_nice = nice(pid, new_nice_val);
    if (old_nice == -1) {
        passed_count ++;
        printf(1, "Passed: can't set nice value to %d for pid %d: out of bounds or invalid PID\n", new_nice_val, pid);
    } else {
        failed_count ++;
        printf(1, "Failed: pid %d, old_nice_value: %d, new_nice_value: %d\n", pid, old_nice, new_nice_val);
    }
    printf(1,"\n");

    // Test 7: Repeat setting to an in-range value to ensure consistency (e.g., set to 4)
    printf(1, "Test 7: Setting nice value to 4\n");
    new_nice_val = 4;
    old_nice = nice(pid, new_nice_val);
    if (old_nice == -1) {
        failed_count ++;
        printf(1, "Failed: can't set nice value to %d for pid %d: out of bounds or invalid PID\n", new_nice_val, pid);
    } else {
        passed_count ++;
        printf(1, "Passed: pid %d, old_nice_value: %d, new_nice_value: %d\n", pid, old_nice, new_nice_val);
    }
    printf(1,"\n");

    printf(1, "All tests completed\n");
    printf(1, "Passed: %d, Failed: %d\n", passed_count, failed_count);
    exit();
}