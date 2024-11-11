#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
    int pid;
    int new_nice;
    if(argc <2 || argc > 3){
        printf(2,"Invalid command\n");
        exit();
    }
    if(argc == 2){
        pid = getpid();
        new_nice = atoi(argv[1]);
        if (argv[1][0] == '-'){
            new_nice = -1 * atoi((char *)(&argv[1][1]));
        }
        else{
            new_nice = atoi(argv[1]);
        }
    }
    else{
        pid = atoi(argv[1]);
        new_nice = atoi(argv[2]);
        if(new_nice < 1 || new_nice > 5){
            printf(2,"Invalid command\n");
            exit();
        }
        if (argv[2][0] == '-'){
            new_nice = -1 * atoi((char *)(&argv[2][1]));
        }
        else{
            new_nice = atoi(argv[2]);
        }
    }

    if (pid < 1){
        printf(2,"Invalid pid\n");
        exit();
    }

    int res = nice(pid,new_nice);
    if (res == -1){
        printf(2,"Invalid nice value or pid\n");
    }
    else {
        printf(2,"pid: %d old_nice_val: %d\n",pid,res);
    }
    exit();
}