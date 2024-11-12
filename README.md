# Assignment Guide

## Setup Instructions

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd <repository-directory>

2. Compile the code:
   ```bash
   make clean && make && make qemu-nox

3. This will open the shell terminal.

# Part 1: Nice Value
## Description
This part demonstrates the usage of the nice system call, which adjusts the priority of a process.

## How to Update Nice Value
In C code:  nice(pid, nice_val);
In Shell: nice <pid> <nice_val>

## Test Cases
There are two test files provided for Part 1:

Task1test1.c - Run using the command:
command - task1test1

Task1test2.c - Run using the command:
command - task1test2



# Part 2: Scheduler
This part tests the Round Robin scheduler with and without priority.

## (1) Round Robin with Priority
Test Cases
Three test files are provided for Round Robin with Priority:

Task2test1.c - Run using the command:
command - task2test1


Task2test2.c - Run using the command:
task2test2

Task2test3.c - Run using the command:
task2test3

## (2) Round Robin without Priority (Default Scheduler)
To test the default Round Robin scheduler (without priority):

Comment out the line #define PRIORITY_SCHEDULER in the proc.c file before the scheduler() method.

Recompile the code with:
make clean && make && make qemu-nox

Run the same test cases as above:
Task2test1.c - Run using the command:
command - task2test1


Task2test2.c - Run using the command:
task2test2

Task2test3.c - Run using the command:
task2test3

## Notes
Ensure that you recompile the code whenever you make changes to proc.c or any other system files.
Make sure to follow the order and instructions for each part to observe the differences between the scheduling methods and nice values.
