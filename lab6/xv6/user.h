struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
void flexSCReg(void);
int flexSCWait(void);
int flexScFork(void);
int flexScExit(void) ;
int flexWait(void);
int flexScPipe(int*);
int flexScWrite(int, const void*, int);
int flexScRead(int, void*, int);
int flexScClose(int);
int flexScKill(int);
int flexScExec(char*, char**);
int flexScOpen(const char*, int);
int flexScMknod(const char*, short, short);
int flexScUnlink(const char*);
int flexScFstat(int fd, struct stat*);
int flexScLink(const char*, const char*);
int flexScMkdir(const char*);
int flexScChdir(const char*);
int flexScDup(int);
int flexScGetpid(void);
char* flexScSbrk(int);
int flexScSleep(int);
int flexScUptime(void);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
