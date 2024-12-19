#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"

// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.

// Fetch the int at addr from the current process.
int
fetchint(uint addr, int *ip)
{
  struct proc *curproc = myproc();

  if(addr >= curproc->sz || addr+4 > curproc->sz)
    return -1;
  *ip = *(int*)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int
fetchstr(uint addr, char **pp)
{
  char *s, *ep;
  struct proc *curproc = myproc();

  if(addr >= curproc->sz)
    return -1;
  *pp = (char*)addr;
  ep = (char*)curproc->sz;
  for(s = *pp; s < ep; s++){
    if(*s == 0)
      return s - *pp;
  }
  return -1;
}

// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
  return fetchint((myproc()->tf->esp) + 4 + 4*n, ip);
}

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size bytes.  Check that the pointer
// lies within the process address space.
int
argptr(int n, char **pp, int size)
{
  int i;
  struct proc *curproc = myproc();
 
  if(argint(n, &i) < 0)
    return -1;
  if(size < 0 || (uint)i >= curproc->sz || (uint)i+size > curproc->sz)
    return -1;
  *pp = (char*)i;
  return 0;
}

int
argintptr(int n, int **pp, int size)
{
  int i;
  struct proc *curproc = myproc();

  if(argint(n, &i) < 0)
    return -1;
  if(size < 0 || (uint)i >= curproc->sz || (uint)i+size > curproc->sz)
    return -1;
  *pp = (int*)i;
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int
argstr(int n, char **pp)
{
  int addr;
  if(argint(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

extern int sys_add_to_strace_dump(void);
extern int sys_chdir(void);
extern int sys_close(void);
extern int sys_dup(void);
extern int sys_exec(void);
extern int sys_exit(void);
extern int sys_fork(void);
extern int sys_fstat(void);
extern int sys_getpid(void);
extern int sys_kill(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_mknod(void);
extern int sys_open(void);
extern int sys_pipe(void);
extern int sys_print_strace_dump(void);
extern int sys_add_to_strace_stat(void);
extern int sys_clear_strace_stats(void);
extern int sys_print_strace_stats(void);
extern int sys_get_strace_ofile(void);
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_set_strace_conf(void);
extern int sys_get_strace_conf(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);

static int (*syscalls[])(void) = {
[SYS_fork]    sys_fork,
[SYS_exit]    sys_exit,
[SYS_wait]    sys_wait,
[SYS_pipe]    sys_pipe,
[SYS_read]    sys_read,
[SYS_kill]    sys_kill,
[SYS_exec]    sys_exec,
[SYS_fstat]   sys_fstat,
[SYS_chdir]   sys_chdir,
[SYS_dup]     sys_dup,
[SYS_getpid]  sys_getpid,
[SYS_sbrk]    sys_sbrk,
[SYS_sleep]   sys_sleep,
[SYS_uptime]  sys_uptime,
[SYS_open]    sys_open,
[SYS_write]   sys_write,
[SYS_mknod]   sys_mknod,
[SYS_unlink]  sys_unlink,
[SYS_link]    sys_link,
[SYS_mkdir]   sys_mkdir,
[SYS_close]   sys_close,
[SYS_set_strace_conf] sys_set_strace_conf,
[SYS_get_strace_conf] sys_get_strace_conf,
[SYS_add_to_strace_dump] sys_add_to_strace_dump,
[SYS_print_strace_dump] sys_print_strace_dump,
[SYS_get_strace_ofile] sys_get_strace_ofile,
[SYS_add_to_strace_stat] sys_add_to_strace_stat,
[SYS_clear_strace_stats] sys_clear_strace_stats,
[SYS_print_strace_stats] sys_print_strace_stats
};

const char* get_sys_call_name(int num){
  const char* sys_call_name[] = {"fork",
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
"get_strace_ofile",
"add_to_strace_stat",
"clear_strace_stats"
};

  return sys_call_name[num-1];
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

unsigned long long rdtsc() {
    unsigned long long tsc;
    __asm__ __volatile__("mfence;rdtsc" : "=A" (tsc):: "memory");
    return tsc;
}


int normalize(double *val) {
    int exponent = 0;
    double value = *val;

    while (value >= 1.0) {
        value /= 10.0;
        ++exponent;
    }

    while (value < 0.1) {
        value *= 10.0;
        --exponent;
    }
    *val = value;
    return exponent;
}

void ftoa_fixed(char *buffer, double value) {  
    int exponent = 0;
    int places = 0;
    static const int width = 10;

    if (value == 0.0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }         

    if (value < 0.0) {
        *buffer++ = '-';
        value = -value;
    }

    exponent = normalize(&value);
    while (exponent > 0) {
        int digit = value * 10;
        *buffer++ = digit + '0';
        value = value * 10 - digit;
        ++places;
        --exponent;
    }

    if (places == 0)
        *buffer++ = '0';

    *buffer++ = '.';

    while (exponent < 0 && places < width) {
        *buffer++ = '0';
        ++exponent;
        ++places;
    }

    while (places < width) {
        int digit = value * 10.0;
        *buffer++ = digit + '0';
        value = value * 10.0 - digit;
        ++places;
    }
    *buffer = '\0';
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ 
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ 
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

int append_string(char* dest, const char* src){
  int len = strlen(src);
  strncpy(dest,src,len);
  return len;
}

void
syscall(void)
{
  int num;
  struct proc *curproc = myproc();

  num = curproc->tf->eax;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    unsigned long long start, end;
    start = rdtsc();
    curproc->tf->eax = syscalls[num]();
    end = rdtsc();
    int strace_conf[5];

    if (curproc->strace_conf[0] == -1) {
        for (int i = 0; i < 5; i++) {
            strace_conf[i] = get_strace_conf(0, i);
        }
    } else {
        for (int i = 0; i < 5; i++) {
            strace_conf[i] = curproc->strace_conf[i];
        }
    }
    char buf[100] = {0};

    ftoa_fixed(buf, (end-start)/(CPU_FREQUENCY * 1000000));
    if(((curproc->pid)>1) && strcmp(curproc->name,"sh") !=0 && strcmp(curproc->name, "strace")!=0 ){
      if(strace_conf[0] == 1 ){
        if(strace_conf[1] == -1 || strace_conf[1] == num){
          if(strace_conf[2] == -1 || (strace_conf[2] == 1 && ((int)(curproc->tf->eax)) >=0) || (strace_conf[2] == 2 && ((int)(curproc->tf->eax)) <0)){
            
            if(strace_conf[3] !=-1){
              add_to_strace_stat(num-1,((int)(curproc->tf->eax) < 0),end-start);
            }
            if(strace_conf[4]!=-1){
              int *file_int_ptr;
              get_strace_ofile(&file_int_ptr);
              struct file* global_strace_ofile = (struct file*)(file_int_ptr);
              char toWrite[150];
              memset(toWrite,0,150);
              int idx=0;
              idx+=append_string((char *)(toWrite+idx),"TRACE: pid = \0");
              char buffer [sizeof(int)*8+1];
              char* pid_str = itoa( curproc->pid,buffer);
              idx+=append_string((char *)(toWrite+idx),pid_str);
              idx+=append_string((char *)(toWrite+idx)," | command_name = ");
              idx+=append_string((char *)(toWrite+idx),curproc->name);
              idx+=append_string((char *)(toWrite+idx)," | syscall = ");
              idx+=append_string((char *)(toWrite+idx),get_sys_call_name(num));
              idx+=append_string((char *)(toWrite+idx)," | return value = ");
              char* ret_str = itoa( curproc->tf->eax,buffer);
              idx+=append_string((char *)(toWrite+idx),ret_str);
              idx+=append_string((char *)(toWrite+idx)," | time = ");
              idx+=append_string((char *)(toWrite+idx),buf);
              idx+=append_string((char *)(toWrite+idx),"\n");
              toWrite[idx] = '\0';
              filewrite(global_strace_ofile, toWrite, strlen(toWrite));
            }
            if(strace_conf[3] !=1 && strace_conf[4]!=1)
              cprintf("TRACE: pid = %d | command_name = %s | syscall = %s | return value = %d \n", curproc->pid, curproc->name, get_sys_call_name(num), curproc->tf->eax);
          }
        }
      }
      add_to_strace_dump(curproc->pid, curproc->name, get_sys_call_name(num), curproc->tf->eax, buf);
    }
  } else {
    cprintf("%d %s: unknown sys call %d\n",
            curproc->pid, curproc->name, num);
    curproc->tf->eax = -1;
  }
}