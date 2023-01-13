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
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);
extern int sys_flexSCReg(void);
extern int sys_flexSCWait(void);
extern int sys_flexScFork  (void);
extern int  sys_flexScExit (void);
extern int  sys_flexWait   (void);
extern int  sys_flexScPipe (void);
extern int  sys_flexScRead (void);
extern int  sys_flexScKill (void);
extern int  sys_flexScExec (void);
extern int  sys_flexScFstat(void);
extern int  sys_flexScChdir(void);
extern int  sys_flexScDup  (void);
extern int  sys_flexScGetpid (void) ;
extern int  sys_flexScSbrk  (void);
extern int  sys_flexScSleep (void);
extern int  sys_flexSc (void);
extern int  sys_flexScOpen  (void);
extern int  sys_flexScWrite (void);
extern int  sys_flexScMknod (void);
extern int  sys_flexScUnlink(void);
extern int  sys_flexScLink  (void); 
extern int  sys_flexScMkdir (void);
extern int  sys_flexScClose (void);


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
[SYS_flexSCReg] sys_flexSCReg,
[SYS_flexSCWait] sys_flexSCWait,
[SYS_flexScFork]      sys_flexScFork  ,
[SYS_flexScExit]      sys_flexScExit  ,
[SYS_flexWait]        sys_flexWait    ,
[SYS_flexScPipe]      sys_flexScPipe  ,
[SYS_flexScRead ]     sys_flexScRead  ,
[SYS_flexScKill]      sys_flexScKill  ,
[SYS_flexScExec ]     sys_flexScExec  ,
[SYS_flexScFstat ]    sys_flexScFstat ,
[SYS_flexScChdir]     sys_flexScChdir ,
[SYS_flexScDup   ]    sys_flexScDup   ,
[SYS_flexScGetpid ]   sys_flexScGetpid , 
[SYS_flexScSbrk   ]   sys_flexScSbrk   ,
[SYS_flexScSleep  ]   sys_flexScSleep  , 
[SYS_flexScUptime ]   sys_flexScUptime ,     
[SYS_flexScOpen  ]    sys_flexScOpen   ,
[SYS_flexScWrite]     sys_flexScWrite  ,
[SYS_flexScMknod ]    sys_flexScMknod  ,
[SYS_flexScUnlink ]   sys_flexScUnlink ,
[SYS_flexScLink ]     sys_flexScLink   ,
[SYS_flexScMkdir ]    sys_flexScMkdir  ,
[SYS_flexScClose  ]   sys_flexScClose  ,


//adding more FlexSC-syscalls
};

int
makeCall(int num)
{
  myproc()->tf->eax = syscalls[num]();
}

void
syscall(void)
{
  int num;
  struct proc *curproc = myproc();

  //check if flexsc is 1 if yes add to syscall pgs dont call syscall
  //how to add? (pointer to next entry?) but how to get address of that free entry in syscall pg
  //curproc->nextEntry += 4 bytes
  //change status
  num = curproc->tf->eax;
  
  if(curproc->isFlexsc && num < 22 && syscalls[num])
  {
    acquire(curproc->sysPage->syspage_lock);

    int found= 0;
    int retFlag = 0; //if 1, status is done, something is wrong 
    for(int i = 0; i<125; i++)
    {
      if(curproc->sysPage->syscalls[i].status == 0)   //found entry
      {
        found = 1;
        curproc->sysPage->syscalls[i].syscall_no = num;
        curproc->sysPage->syscalls[i].return_val = 0;


        //checking arguments;
        switch( num)
        {
          case 1: 
              curproc->sysPage->syscalls[i].args_no = 0;
              break;
          case 2:
              curproc->sysPage->syscalls[i].args_no = 0;
              break;
          case 3:
              curproc->sysPage->syscalls[i].args_no = 0;
              break;
          case 4:
              curproc->sysPage->syscalls[i].args_no = 1;
              
              int *fd;

              if(argptr(0, (void*)&fd, 2*sizeof(fd[0])) < 0)
                retFlag = 1;

              curproc->sysPage->syscalls[i].arg1 = fd;
              break;
          case 5:
              curproc->sysPage->syscalls[i].args_no = 3;

              struct file *f;
              int n;
              char *p;

              if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0)
                retFlag = 1;

              curproc->sysPage->syscalls[i].arg1 = f;
              curproc->sysPage->syscalls[i].arg2 = &n;
              curproc->sysPage->syscalls[i].arg3 = p;
              break;
          case 6:
              curproc->sysPage->syscalls[i].args_no = 1;

              int pid;
              if(argint(0, &pid) < 0)
                retFlag = 1;

              curproc->sysPage->syscalls[i].arg1 = &pid;
              break;
          case 7:
              curproc->sysPage->syscalls[i].args_no = 2;

              char *path;
              uint uargv;
              if(argstr(0, &path) < 0 || argint(1, (int*)&uargv) < 0){
                retFlag = 1;
              }

              curproc->sysPage->syscalls[i].arg1 = path;
              curproc->sysPage->syscalls[i].arg2 = &uargv;
              
              break;
          case 8:
              curproc->sysPage->syscalls[i].args_no = 2;

              struct file *f;
              struct stat *st;

              if(argfd(0, 0, &f) < 0 || argptr(1, (void*)&st, sizeof(*st)) < 0)
                retFlag  = 1;

              curproc->sysPage->syscalls[i].arg1 = f;
              curproc->sysPage->syscalls[i].arg2 = st;
              
              break;
          case 9:   //
              curproc->sysPage->syscalls[i].args_no = 1;

              char *path;
              if (argstr(0, &path)< 0)
              {
                curproc->sysPage->syscalls[i].return_val = -1 ;
              }

              curproc->sysPage->syscalls[i].arg1 = path;

              break;
          case 10:
              curproc->sysPage->syscalls[i].args_no = 1;

              struct file *f;
              if(argfd(0, 0, &f) < 0)
                retFlag = 1;
              
              curproc->sysPage->syscalls[i].arg1 = f;
              break;
          case 11:
              curproc->sysPage->syscalls[i].args_no = 0;
              break;

          case 12:
              curproc->sysPage->syscalls[i].args_no = 1;

              int n;
              if(argint(0, &n) < 0)
                retFlag = 1;

              curproc->sysPage->syscalls[i].arg1= &n;

              break;
          case 13:
              curproc->sysPage->syscalls[i].args_no = 1;
              int n;
              if(argint(0, &n) < 0)
                retFlag = 1;

              curproc->sysPage->syscalls[i].arg1= &n;

              break;

          case 14:
              curproc->sysPage->syscalls[i].args_no = 0;
              break;
          case 15:
              curproc->sysPage->syscalls[i].args_no = 2;

              char* path;
              int omode;
              if(argstr(0, &path) < 0 || argint(1, &omode) < 0)
                retFlag = 1;

               curproc->sysPage->syscalls[i].arg1= path;
               curproc->sysPage->syscalls[i].arg2= &omode;
              
              break;
          case 16:
              curproc->sysPage->syscalls[i].args_no = 3;
              struct file *f;
              int n;
              char *p;

              if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0)
                retFlag = 1;
              
              curproc->sysPage->syscalls[i].arg1= f;
              curproc->sysPage->syscalls[i].arg2= p;
              curproc->sysPage->syscalls[i].arg3= &n;

              break;
          case 17:    //mark 
              curproc->sysPage->syscalls[i].args_no = 3;
              
              char *path;
              int major, minor;

              if((argstr(0, &path)) < 0 || argint(1, &major) < 0 || argint(2, &minor) < 0 )
              {
                curproc->sysPage->syscalls[i].return_val = -1 ;
              }
              
              curproc->sysPage->syscalls[i].arg1= path;
              curproc->sysPage->syscalls[i].arg2= &major;
              curproc->sysPage->syscalls[i].arg3= &minor;
              break;
          case 18:
              curproc->sysPage->syscalls[i].args_no = 1;

              char *path ;

              if(argstr(0, &path) < 0)
                  retFlag = 1;

              curproc->sysPage->syscalls[i].arg1= path;

              break ;
          case 19:
              curproc->sysPage->syscalls[i].args_no = 2;

                char *new, *old;

              if(argstr(0, &old) < 0 || argstr(1, &new) < 0)
                retFlag = 1;

              curproc->sysPage->syscalls[i].arg1= old;
              curproc->sysPage->syscalls[i].arg2= new;

              break ;
          case 20:  //mark
                curproc->sysPage->syscalls[i].args_no = 1;

                char *path ;
                  
                if(argstr(0, &path) < 0 )
                  curproc->sysPage->syscalls[i].return_val = -1;

                curproc->sysPage->syscalls[i].arg1= path;

                break ;
          case 21:
              curproc->sysPage->syscalls[i].args_no = 1;

                int fd;
                struct file *f;

                if(argfd(0, &fd, &f) < 0)
                  retFlag = 1 ;

                curproc->sysPage->syscalls[i].arg1= fd ;
              break ;
          default :
              break;
        }

        if(retFlag == 1)
        {
          curproc->sysPage->syscalls[i].status = 2;
          curproc->sysPage->syscalls[i].return_val = -1 ;
        }
        else
        {
          curproc->sysPage->syscalls[i].status = 1;
        }
        break ;
      }
    }

    if(found == 0)
    {
      // may be we have to wait ??
      cprintf("Didn't find free page") ;
    }

    release(curproc->sysPage->syspage_lock);
  }
  else
  {
      if(num > 0 && num < NELEM(syscalls) && syscalls[num]) 
      {
        curproc->tf->eax = syscalls[num]();
      } else 
      {
        cprintf("%d %s: unknown sys call %d\n",
                curproc->pid, curproc->name, num);
        curproc->tf->eax = -1;
      }
  }

  
}
