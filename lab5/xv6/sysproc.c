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


/* The v2paddr() system call 
      --takes a virtual address in the address space of the process calling the system call as an argument,
      --and returns the corresponding physical address.
-> If the virtual address passed is invalid, then it returns -1 ;
-> If the page corresponding to VA is not present in Physical Memory, then it returns -1; 
*/
int
sys_v2paddr(void)
{
  // getting the value of 1st argument i.e. virtual address
  uint* vaddr ;
  uint paddr ;
  int arg_num = 0 ;
  uint sample_no = 1 ;
  vaddr = &sample_no ;
  paddr = sample_no ;

  // now in vaddr - system call argument virtual address is stored
  // *vaddr = va
  if( argint(arg_num,(int*)(vaddr)) < 0 )
  {
      cprintf("Error occured while fetching the system call argument '%d' for 'v2paddr' system call\n\n" , arg_num) ;
      return -1 ;
  }

  // checking whether virtual address is beyond the process address space or not  
  // If it is beyond then it is an invalid virtual address in that processes' address space
  struct proc *curproc = myproc() ;     // curproc - process which called "v2paddr" system call

  if((*vaddr) >= curproc->sz || ((*vaddr) +4) >= curproc->sz)
  {
    cprintf("Invalid virtual address\n") ;
    cprintf("Given VA:0x%x is not in the address space of process calling the system call 'v2paddr'\n\n" , *vaddr);
    return -1 ;
  }

  //pade directory 
  pde_t* pgdir = curproc->pgdir ;
  pde_t pde ;    // page dir entry 

  pde = pgdir[PDX(*vaddr)] ;

  // if pde's user bit is 0
  if( !(pde & PTE_U))
  {
    cprintf("For VA:0x%x , user not allowed to use the page in page directory\n\n" , *vaddr) ;
    return -1 ;
  }

  // pde can have present bit as either 0 or 1
  if(pde & PTE_P)      // if pde's present bit is 1 
  {
    // page table - pgtab
    pde_t* pgtab = (pte_t*)P2V(PTE_ADDR(pde));   

    // page table entry
    pde_t pte = pgtab[PTX(*vaddr)] ;

    if( !(pte & PTE_U))
    {
      cprintf("For VA:0x%x , user not allowed to use the page in page table\n" , *vaddr) ;
      return -1 ;
    }

    if( !(pte & PTE_P))
    {
      cprintf("For VA:0x%x , page table entry's present bit is 0\n" , *vaddr) ;
      cprintf("Corresponding physical address is not accessible from RAM\n\n") ;
      return -1 ;
    }

    // After getting page table entry , 
    // physical address is obtained to adding last 12 bits of virtual address to first 20 bits of page table entry
    paddr = PTE_ADDR(pte) | PTE_FLAGS(*vaddr) ;

    return paddr ;
  }
  else    // pde entry's bit is 0
  {
    cprintf("For VA:0x%x , page directory entry's present bit is 0\n" , *vaddr) ;
    cprintf("Corresponding physical address is not accessible from RAM\n\n") ;
    return -1 ;
  }
}
