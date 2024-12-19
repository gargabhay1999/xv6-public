#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int sys_set_strace_conf(void) {
    int pid, idx, val;

    if (argint(0, &pid) < 0 ||
        argint(1, &idx) < 0 ||
        argint(2, &val) < 0) {
        return -1;
    }

    return set_strace_conf(pid, idx, val);
}


int sys_get_strace_conf(void) {
    int pid, idx;

    if (argint(0, &pid) < 0 ||
        argint(1, &idx) < 0) {
        return -1;
    }

    return get_strace_conf(pid, idx);
}


int sys_add_to_strace_dump(void) {
    int pid, return_value;
    char *command_name, *sys_call_name, *time;

    if (argint(0, &pid) < 0 ||
        argstr(1, &command_name) < 0 ||
        argstr(2, &sys_call_name) < 0 ||
        argint(3, &return_value) < 0 ||
        argstr(4, &time) < 0) {
        return -1;
    }

    return add_to_strace_dump(pid, command_name, sys_call_name, return_value, time);
}

int
sys_print_strace_dump(void)
{
  return print_strace_dump();
}

int
sys_get_strace_ofile(void){
  int* fp;
  if(argintptr(0, &fp, 0) < 0){
    return -1;
  }
  return get_strace_ofile(&fp);
}

int sys_clear_strace_stats(void){
  return clear_strace_stats();
}


int sys_add_to_strace_stat(void) {
    int sys_call_num, error, ticks;

    if (argint(0, &sys_call_num) < 0 ||
        argint(1, &error) < 0 ||
        argint(2, &ticks) < 0) {
        return -1;
    }

    return add_to_strace_stat(sys_call_num, error, ticks);
}

int sys_print_strace_stats(void){
  return print_strace_stats();
}