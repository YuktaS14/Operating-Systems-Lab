// Per-CPU state
struct cpu {
  uchar apicid;                // Local APIC ID
  struct context *scheduler;   // swtch() here to enter scheduler
  struct taskstate ts;         // Used by x86 to find stack for interrupt
  struct segdesc gdt[NSEGS];   // x86 global descriptor table
  volatile uint started;       // Has the CPU started?
  int ncli;                    // Depth of pushcli nesting.
  int intena;                  // Were interrupts enabled before pushcli?
  struct proc *proc;           // The process running on this cpu or null
  int syscall_entryNum ;       // useful only for cpu3 
};

extern struct cpu cpus[NCPU];
extern int ncpu;

//PAGEBREAK: 17
// Saved registers for kernel context switches.
// Don't need to save all the segment registers (%cs, etc),
// because they are constant across kernel contexts.
// Don't need to save %eax, %ecx, %edx, because the
// x86 convention is that the caller has saved them.
// Contexts are stored at the bottom of the stack they
// describe; the stack pointer is the address of the context.
// The layout of the context matches the layout of the stack in swtch.S
// at the "Switch stacks" comment. Switch doesn't save eip explicitly,
// but it is on the stack and allocproc() manipulates it.
struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// 8 * 4 = 32 bytes - SYSCALL ENTRY 
struct syscall_entry {
  int syscall_no ;  // system call number
  int args_no ;     // no of arguments to syscall
  int status;    // status : 0 -> FREE , 1 -> SUBMITTED , 2 -> BUSY , 3 -> DONE

  void* arg1;      
  void* arg2;
  void* arg3;
  int thread_ID ;     // tID of thread which executes this system call
  void* return_val ;   // return value of system call
};

// SYSCALL_PAGE - 4096 byes = (32)*(125) + rest space for lock etc..
// syscall page of a process requires page lock
struct syscall_page {
  struct spinlock *syspage_lock;
  struct syscall_entry syscalls[125] ;
};

  
// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)

  int isFlexsc ;               // flag to know whether flexible system calls are needed or not
                               // isFlexsc = 1  -> yes , flexsc syscalls needed
  
  struct syscall_page *sysPage ;              //pointer to syscall pg created
};

// Process memory is laid out contiguously, low addresses first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap
int makeCall(int n);
