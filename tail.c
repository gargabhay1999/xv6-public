#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define DEFAULT_LINES 10
#define BUF_SIZE 4096

void tail(int fd, int num_lines) {
    char buffer[BUF_SIZE];
    char lines[BUF_SIZE * 2];
    int total_size = 0;
    int read_size;

    // Read the entire file/input into memory
    while ((read_size = read(fd, buffer, sizeof(buffer))) > 0) {
        if (total_size + read_size > sizeof(lines)) {
            printf(1, "tail: file too large\n");
            exit();
        }
        memmove(lines + total_size, buffer, read_size);
        total_size += read_size;
    }

    if (read_size < 0) {
        printf(1, "tail: read error\n");
        exit();
    }

    // Count newlines to find the starting point for printing
    int newline_count = 0;
    for (int i = total_size - 1; i >= 0; i--) {
        if (lines[i] == '\n') {
            newline_count++;
        }
        if (newline_count > num_lines) {
            // Start printing from here
            write(1, lines + i + 1, total_size - i - 1);
            return;
        }
    }

    // If fewer lines than requested, print the entire content
    write(1, lines, total_size);
}

int main(int argc, char *argv[]) {
    int num_lines = DEFAULT_LINES;
    int fd;

    // Parse command-line arguments
    if (argc > 1 && argv[1][0] == '-') {
        num_lines = atoi(&argv[1][1]);

        if (num_lines <= 0) {
            printf(1, "tail: invalid number of lines\n");
            exit();
        }
        argc--;
        argv++;
    }

    if (argc < 2) {
        // Read from standard input
        tail(0, num_lines);
    } else {
        // Read from the specified file
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            printf(1, "tail: cannot open %s\n", argv[1]);
            exit();
        }
        tail(fd, num_lines);
        close(fd);
    }

    exit();
}
