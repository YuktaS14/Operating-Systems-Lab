# Implementation of exception-less system calls on xv6

#### Adding system call- FlexSc_register():
  The processes which use exceptionless system calls need to register using this system call. Registration mainly involves 2 steps:
- Mapping one or more syscall pages into user-space virtual memory space.
- Spawning one syscall thread per entry in syscall pages.

#### Syscall Pages:
- When a process is created, memory is allocated for its syscall pages using kalloc function.
- A flag bit is kept to tell the user that this space in address space is allocated for only syscall pages
- Each entry in this page contains system call number, its arguments, total number of arguments, status( whether the corresonding entry is free/submitted/busy/done)
- So size per entry in syscalll page can be found as:
-- for syscall number (considering 22 system calls + 22 newly added Flexsc system calls) : 7 bits
-- number of arguments (assuming any system call takes maximum 3 arguments): 2 bits.
-- arguments : (at max) 3*4 = 12 bytes
-- status : 3 bits
-- return value : 4 bytes (at max)
 
- create a separate function which will find a free entry (may be need to maintain a pointer to keep track of entries filled) in syscall pages and write correspondingly in that entry when a system call comes. Change the status of that entry from free to submitted (i.e waiting for execution)

#### 
- Initially we create Flexsc_(systemcalls) corresponding to each systemcalls.
- so whenever a system call comes these Flexsc(systemcall) will be called instead of directly calling system calls. We can then disable these system calls (at the end erase int instruction - initcode.S) upon successful implementation.


##### (Assuming we implemented user level and kernel level threads):
- We need to use locks whenever new entry is getting added to syscall pages, because syscall pages is a shared memory.
- Whenever a system call comes the user thread will go to sleep and then next user thread wakes up and continues further execution.
- Syscall entries in the pages are executed by kernel threads. It accordingly changes the status from submitted -> busy -> done, and writes the return value to syscall pages.

#### Thread Scheduling:
- If all the user threads are blocked, then thread schedular will find if any syscall entry has status done, if yes then it will give the return value to the corresponding user thread using thread id (One of the approach can be to reserve specific bits of the syscall page entry for the thread id).

- The schedular will then change the status of the page entry to free and discard the values in that entry.

- If user threads are blocked and schedular is unable to find any syscall entry with status as done, then Flexsc_wait() system call (newly implemented) is called. This is implemented as exception based system call. It communicates to kernel that user threads can't proceed by putting the kernel visible thread to sleep until one of the pending system calls has completed.

- Only one syscall thread (kernel level) is active per application and core at any given point in time.

- For improving efficiency and using locality concept, specific cores can be made to implement specific syscalls. Syscalls issued on one core can be executed on different core.


##### DS
- queue- syscalls execution --- 
- queue- syscall threads...if 1 goes to sleep then it wake the other before ---()
- task-struct- (stack) for keeping track of syscall threads
