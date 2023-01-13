#include "syscall.h"
#include "traps.h"

#define SYSCALL(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
SYSCALL(exit)
SYSCALL(wait)
SYSCALL(pipe)
SYSCALL(read)
SYSCALL(write)
SYSCALL(close)
SYSCALL(kill)
SYSCALL(exec)
SYSCALL(open)
SYSCALL(mknod)
SYSCALL(unlink)
SYSCALL(fstat)
SYSCALL(link)
SYSCALL(mkdir)
SYSCALL(chdir)
SYSCALL(dup)
SYSCALL(getpid)
SYSCALL(sbrk)
SYSCALL(sleep)
SYSCALL(uptime)
SYSCALL(flexSCReg)
SYSCALL(flexSCWait)
SYSCALL(flexScFork)
SYSCALL(flexScExit)
SYSCALL(flexWait)
SYSCALL(flexScPipe)
SYSCALL(flexScRead)
SYSCALL(flexScWrite)
SYSCALL(flexScClose)
SYSCALL(flexScKill)
SYSCALL(flexScExec)
SYSCALL(flexScOpen)
SYSCALL(flexScMknod)
SYSCALL(flexScUnlink)
SYSCALL(flexScFstat)
SYSCALL(flexScLink)
SYSCALL(flexScMkdir)
SYSCALL(flexScChdir)
SYSCALL(flexScDup)
SYSCALL(flexScGetpid)
SYSCALL(flexScSbrk)
SYSCALL(flexScSleep)
SYSCALL(flexScUptime)