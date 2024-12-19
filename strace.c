#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
char buf[512];

int get_sys_call_number(char* sys_call_name){
  const char* sys_call_names[] = {"fork",
  "exit",
  "wait",
  "pipe",
  "read",
  "kill",
  "exec",
  "fstat",
  "chdir",
  "dup",
  "getpid",
  "sbrk",
  "sleep",
  "uptime",
  "open",
  "write",
  "mknod",
  "unlink",
  "link",
  "mkdir",
  "close",
  "set_strace_conf",
  "get_strace_conf",
  "add_to_strace_dump",
  "print_strace_dump",
  "add_to_strace_stat",
  "clear_strace_stats",
  "print_strace_stats"
  };
  int i=0;
  for(i=0;i<23;i++){
    if (strcmp(sys_call_name,sys_call_names[i])==0){
      return i+1;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
    int strace_conf[] = {-1, -1, -1, -1, -1};

    // Validate arguments
    if (argc < 2) {
        printf(1, "Invalid arguments. Use strace on/off\n");
        exit();
    }

    int i = 1;
    int pid = 0; // 0 for global configuration

    // Handle primary commands
    if (strcmp(argv[i], "on") == 0) {
        strace_conf[0] = 1; // Enable global strace
        i++;
    } else if (strcmp(argv[i], "off") == 0) {
        strace_conf[0] = 0; // Disable global strace
        i++;
    } else if (strcmp(argv[i], "dump") == 0) {
        print_strace_dump(); // Print the current strace dump
        i++;
    }

    // Process additional options
    for (; i < argc; i++) {
        if (strcmp(argv[i], "run") == 0 || strcmp(argv[i], "-c") == 0) {
            strace_conf[0] = 1; // Enable process-specific strace
            if (strcmp(argv[i], "-c") == 0) {
                strace_conf[3] = 1; // Collect stats
            }
            pid = 1; // Indicate process-specific strace
            break;
        } else if (strcmp(argv[i], "-e") == 0) {
            int syscall_num = get_sys_call_number(argv[++i]);
            if (syscall_num > 0) {
                strace_conf[1] = syscall_num; // Set syscall whitelist
            } else {
                printf(1, "Invalid Syscall name\n");
                exit();
            }
        } else if (strcmp(argv[i], "-s") == 0) {
            strace_conf[2] = 1; // Log successful calls only
        } else if (strcmp(argv[i], "-f") == 0) {
            strace_conf[2] = 2; // Log failed calls only
        } else if (strcmp(argv[i], "-o") == 0) {
            int fd = open(argv[++i], O_CREATE | O_RDWR);
            if (fd < 0) {
                printf(1, "Failed to open file: %s\n", argv[i]);
                exit();
            }
            strace_conf[4] = fd; // Set output file descriptor
        } else {
            printf(1, "Invalid arguments. Use strace on/off/run <cmd>\n");
            exit();
        }
    }

    // Handle process-specific strace
    if (pid != 0) {
        if (fork() == 0) { // Child process
            pid = getpid();
        } else { // Parent process
            wait();
            if (strace_conf[3] == 1) {
                print_strace_stats();
            }
            exit();
        }
    }

    // Apply configurations
    for (int j = 0; j < 5; j++) {
        if (strace_conf[j] != -1) {
            if (j != 4) {
                set_strace_conf(pid, j, strace_conf[j]);
            } else {
                set_strace_conf(getpid(), j, strace_conf[j]);
            }
        }
    }

    // Execute the specified command
    if (pid != 0) {
        char **new_args = &argv[i + 1];
        exec(argv[i + 1], new_args);
        printf(1, "Failed to execute command: %s\n", argv[i + 1]);
    }

    exit();
}
