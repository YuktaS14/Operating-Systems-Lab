#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "stat.h"
#include "fs.h"
#include "file.h"
#include "fcntl.h"

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

int 
sys_flexSCReg(void)
{
  //assuming flexSc is enabled by all processes
  myproc()->isFlexsc = 1;
  if(myproc()->isFlexsc)
  {
    myproc()->sysPage = (struct syscall_page*)kalloc();
  }
  //if 1 then create syscall pages
  //curproc -> syscallPg = ( if kalloc() != 0)
  //else ??

  //considering 125 syscall entries in one page
  acquire(myproc()->sysPage->syspage_lock);
  for(int i = 0; i<125; i++)
  {
    myproc()->sysPage->syscalls[i].status = 0; //free
  }
  release(myproc()->sysPage->syspage_lock);
  return 0;
}

int
sys_flexSCWait(void)
{
  //schedular of cpu3 to start working
  //all syscallls
  return 0;
}


int sys_flexScFork  (void)
{
  //set it as busy at scheduler - it is left to be done ???
  int id = mycpu()->syscall_entryNum;
  int pid = fork();
  myproc()->sysPage->syscalls[id].return_val = pid;

  return pid;
}

int  sys_flexScExit (void){
  exit();
  return 0;
}
int  sys_flexWait   (void){
  return wait();
}
int  sys_flexScPipe (void){
  int *fd;
  struct file *rf, *wf;
  int fd0, fd1;

  int retVal = 0;
  int id = mycpu()->syscall_entryNum;
  
  fd = (int *)myproc()->sysPage->syscalls[id].arg1;

  if(pipealloc(&rf, &wf) < 0)
    retVal = -1;
  fd0 = -1;
  if((fd0 = fdalloc(rf)) < 0 || (fd1 = fdalloc(wf)) < 0){
    if(fd0 >= 0)
      myproc()->ofile[fd0] = 0;
    fileclose(rf);
    fileclose(wf);
    retVal = -1;
  }
  fd[0] = fd0;
  fd[1] = fd1;

  myproc()->sysPage->syscalls[id].return_val = retVal;
  return 0;
}
int  sys_flexScRead (void){
    struct file *f;
    int n;
    char *p;

    int id = mycpu()->syscall_entryNum;
    f = (struct file *)myproc()->sysPage->syscalls[id].arg1;
    n = *((int *)myproc()->sysPage->syscalls[id].arg2);
    p = (char *)myproc()->sysPage->syscalls[id].arg3;

    int retVal = fileread(f, p, n);

    myproc()->sysPage->syscalls[id].return_val = retVal;
    return retVal;
}

// can kill be asynchronous ???
int  sys_flexScKill (void)
{
  int pid;
  int id = mycpu()->syscall_entryNum;

  int retVal = kill(pid);
  myproc()->sysPage->syscalls[id].return_val = retVal;
  return retVal;
}

int  sys_flexScExec (void){
  char *path, *argv[MAXARG];
  int i;
  uint uargv, uarg;
  int retVal = 0;

  int id = mycpu()->syscall_entryNum;
  
  path = (char *)myproc()->sysPage->syscalls[id].arg1;
  uargv = *((int *)myproc()->sysPage->syscalls[id].arg2);

  
  memset(argv, 0, sizeof(argv));
  for(i=0;; i++){
    if(i >= NELEM(argv))
      retVal = -1;
    if(fetchint(uargv+4*i, (int*)&uarg) < 0)
      retVal = -1;
    if(uarg == 0){
      argv[i] = 0;
      break;
    }
    if(fetchstr(uarg, &argv[i]) < 0)
      retVal = -1;
  }
  
  if(retVal != -1)
    retVal = exec(path, argv);

  myproc()->sysPage->syscalls[id].return_val = retVal;
  return retVal;
}

//
int  sys_flexScWrite (void)
{
  struct file *f;
  int n;
  char *p;

  int id = mycpu()->syscall_entryNum;
  f = (struct file*)(myproc()->sysPage->syscalls[id].arg1) ;
  p = (char *)(myproc()->sysPage->syscalls[id].arg2) ;
  n = *(int *)(myproc()->sysPage->syscalls[id].arg3) ;

  int retVal =  filewrite(f, p, n);
  myproc()->sysPage->syscalls[id].return_val = retVal ;
  return retVal ;
}

int  sys_flexScMknod (void)
{
  struct inode *ip;
  char *path;
  int major, minor;

  int id = mycpu()->syscall_entryNum;
  path = (char *)(myproc()->sysPage->syscalls[id].arg1) ;
  major = *(int *)(myproc()->sysPage->syscalls[id].arg2) ;
  minor = *(int *)(myproc()->sysPage->syscalls[id].arg3) ;

  begin_op();
  if( myproc()->sysPage->syscalls[id].return_val == -1 ||(ip = create(path, T_DEV, major, minor)) == 0){
    end_op();
    return -1;
  }
  iunlockput(ip);
  end_op();
  myproc()->sysPage->syscalls[id].return_val == 0;
  return 0;
}



int  sys_flexScUnlink(void)
{
  struct inode *ip, *dp;
  struct dirent de;
  char name[DIRSIZ], *path;
  uint off;
  
  int id = mycpu()->syscall_entryNum;
  path = (char *)(myproc()->sysPage->syscalls[id].arg1) ;

  begin_op();
  if((dp = nameiparent(path, name)) == 0){
    end_op();
    myproc()->sysPage->syscalls[id].return_val = -1;
    return -1;
  }

  ilock(dp);

  // Cannot unlink "." or "..".
  if(namecmp(name, ".") == 0 || namecmp(name, "..") == 0)
    goto bad;

  if((ip = dirlookup(dp, name, &off)) == 0)
    goto bad;
  ilock(ip);

  if(ip->nlink < 1)
    panic("unlink: nlink < 1");
  if(ip->type == T_DIR && !isdirempty(ip)){
    iunlockput(ip);
    goto bad;
  }

  memset(&de, 0, sizeof(de));
  if(writei(dp, (char*)&de, off, sizeof(de)) != sizeof(de))
    panic("unlink: writei");
  if(ip->type == T_DIR){
    dp->nlink--;
    iupdate(dp);
  }
  iunlockput(dp);

  ip->nlink--;
  iupdate(ip);
  iunlockput(ip);

  end_op();

  myproc()->sysPage->syscalls[id].return_val = 0;
  return 0;

bad:
  iunlockput(dp);
  end_op();
  myproc()->sysPage->syscalls[id].return_val = -1;
  return -1;
}


int  sys_flexScLink  (void)
{
  char name[DIRSIZ], *new, *old;
  struct inode *dp, *ip;

  int id = mycpu()->syscall_entryNum;
  old = (const char *)(myproc()->sysPage->syscalls[id].arg1) ;
  new = (const char *)(myproc()->sysPage->syscalls[id].arg2) ;

  begin_op();
  if((ip = namei(old)) == 0){
    end_op();
    myproc()->sysPage->syscalls[id].return_val = -1 ;
    return -1;
  }

  ilock(ip);
  if(ip->type == T_DIR){
    iunlockput(ip);
    end_op();
    myproc()->sysPage->syscalls[id].return_val = -1 ;
    return -1;
  }

  ip->nlink++;
  iupdate(ip);
  iunlock(ip);

  if((dp = nameiparent(new, name)) == 0)
    goto bad;
  ilock(dp);
  if(dp->dev != ip->dev || dirlink(dp, name, ip->inum) < 0){
    iunlockput(dp);
    goto bad;
  }
  iunlockput(dp);
  iput(ip);

  end_op();
  myproc()->sysPage->syscalls[id].return_val = 0 ;
  return 0;

bad:
  ilock(ip);
  ip->nlink--;
  iupdate(ip);
  iunlockput(ip);
  end_op();
  myproc()->sysPage->syscalls[id].return_val = -1 ;
  return -1;
}


int  sys_flexScMkdir (void)
{
  char *path;
  struct inode *ip;
  int id = mycpu()->syscall_entryNum;
  path = (char *)(myproc()->sysPage->syscalls[id].arg1) ;

  begin_op();
  
  if(myproc()->sysPage->syscalls[id].return_val == -1 || (ip = create(path, T_DIR, 0, 0)) == 0){
    end_op();
    return -1;
  }
  iunlockput(ip);
  end_op();
  myproc()->sysPage->syscalls[id].return_val = 0;
  return 0;
}

int  sys_flexScClose (void)
{
  int fd;
  struct file *f;

  int id = mycpu()->syscall_entryNum;
  fd = *(int *)(myproc()->sysPage->syscalls[id].arg1) ;
  
  myproc()->ofile[fd] = 0;
  fileclose(f);

  myproc()->sysPage->syscalls[id].return_val = 0;
  return 0;
}

int  sys_flexScFstat(void)
{
  struct file *f;
  struct stat *st;
  int id = mycpu()->syscall_entryNum;

  f = (struct file *)myproc()->sysPage->syscalls[id].arg1;
  st = (struct stat *)myproc()->sysPage->syscalls[id].arg2;

  int retVal = filestat(f, st);
  myproc()->sysPage->syscalls[id].return_val = retVal;
  
  return retVal;
}

int  sys_flexScChdir(void)
{
  char *path;
  struct inode *ip;
  struct proc *curproc = myproc();

  int id = mycpu()->syscall_entryNum;  
  path = (char *)myproc()->sysPage->syscalls[id].arg1;
  
  int returnVal = 0;
  begin_op();
  if((myproc()->sysPage->syscalls[id].return_val == -1) || (ip = namei(path)) == 0){
    end_op();
    returnVal = -1;
  }
  ilock(ip);
  if(ip->type != T_DIR){
    iunlockput(ip);
    end_op();
    returnVal = -1;
  }
  iunlock(ip);
  iput(curproc->cwd);
  end_op();
  curproc->cwd = ip;

  if(returnVal != -1)
    returnVal = 0;

  myproc()->sysPage->syscalls[id].return_val = returnVal;
  return returnVal;  
}
int  sys_flexScDup  (void)
{
  struct file *f;
  int fd;

  int id = mycpu()->syscall_entryNum;  

  f = *((int *)myproc()->sysPage->syscalls[id].arg1);
  int retVal = 0;
  if((fd=fdalloc(f)) < 0)
    retVal = -1;
  filedup(f);

  if(retVal != -1)
    retVal = fd;
  
  myproc()->sysPage->syscalls[id].return_val = retVal;

  return retVal;
}
int  sys_flexScGetpid (void)
{
  int id = mycpu()->syscall_entryNum;  

  int retVal = myproc()->pid;
  myproc()->sysPage->syscalls[id].return_val = retVal;

  return retVal;
}
int  sys_flexScSbrk  (void)
{
  int addr;
  int n;

  int id = mycpu()->syscall_entryNum; 
  int retVal = 0; 

  n = *((int *)myproc()->sysPage->syscalls[id].arg1);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    retVal = -1;
  
  if(retVal != -1)
    retVal = addr;
  
  n = *((int *)myproc()->sysPage->syscalls[id].arg1);
  myproc()->sysPage->syscalls[id].return_val = retVal;

  return retVal;
}
int  sys_flexScSleep (void)
{
  int n;
  uint ticks0;

  int id = mycpu()->syscall_entryNum; 

  n = *((int *)myproc()->sysPage->syscalls[id].arg1);
  int retVal = 0;

  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      retVal = -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  
  myproc()->sysPage->syscalls[id].return_val = retVal;
  return retVal;
}
int  sys_flexScUptime(void){
  uint xticks;

  int id = mycpu()->syscall_entryNum; 

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  myproc()->sysPage->syscalls[id].return_val = xticks;

  return xticks;
}
int  sys_flexScOpen  (void){
  char *path;
  int fd, omode;
  struct file *f;
  struct inode *ip;

  int id = mycpu()->syscall_entryNum; 

  path = (char *)myproc()->sysPage->syscalls[id].arg1;
  omode = *((int *)myproc()->sysPage->syscalls[id].arg2);

  int retVal = 0;

  begin_op();

  if(omode & O_CREATE){
    ip = create(path, T_FILE, 0, 0);
    if(ip == 0){
      end_op();
      retVal = -1;
    }
  } else {
    if((ip = namei(path)) == 0){
      end_op();
      retVal = -1;
    }
    ilock(ip);
    if(ip->type == T_DIR && omode != O_RDONLY){
      iunlockput(ip);
      end_op();
      retVal = -1;
    }

  if((f = filealloc()) == 0 || (fd = fdalloc(f)) < 0){
  if(f)
    fileclose(f);
  iunlockput(ip);
  end_op();
  retVal = -1;
  }
  iunlock(ip);
  end_op();

  f->type = FD_INODE;
  f->ip = ip;
  f->off = 0;
  f->readable = !(omode & O_WRONLY);
  f->writable = (omode & O_WRONLY) || (omode & O_RDWR);

  if(retVal != -1)
    retVal = fd;
  
  myproc()->sysPage->syscalls[id].return_val = fd;
  return retVal;
}
