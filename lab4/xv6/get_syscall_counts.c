#include "types.h"
#include "user.h"
#include "syscall.h"

int
main(int argc, char *argv[])
{
    printf(1, "\ninitial fork count of parent %d\n", getCount(SYS_fork));

    if (fork() == 0) 
    {
        int childpid = fork() ;
        char *msg[] =  { "I am alive :)\n" };

        if(childpid == 0)
        {    
            if (exec("echo" , msg) < 0)
            {
                printf(1,"\nexec echo failed\n") ;
            }
            printf(1, "exec count of inner child : %d\n" , getCount(SYS_exec)) ;
            sleep(2) ;
            printf(1, "sleep count of inner child : %d\n" , getCount(SYS_sleep)) ;
            printf(1, "fork count of inner child : %d\n" , getCount(SYS_fork)) ;
            printf(1, "write count of inner child : %d\n" , getCount(SYS_write)) ;
            printf(1, "Inner child is running still\n") ;
        }
        else
        {
            sleep(3) ;
            kill(childpid) ;
            printf(1, "\n\nKilled the inner child in the middle of its execution :(\n\n") ;
            wait() ;
            
            printf(1 , "fork count of child process : %d\n",getCount(SYS_fork)) ;
            printf(1 , "sleep count of child process : %d\n",getCount(SYS_sleep)) ;
            printf(1 , "wait count of child process : %d\n",getCount(SYS_wait)) ;
            printf(1, "write count of child process : %d\n" , getCount(SYS_write)) ;
        }
            
    } else 
    {
        wait();
        sleep(5);
       
        int fd = open("sample", 0);

        if(fd < 0)
        {
            printf(1, "\nopen sample failed \n");
        }
        else
        {
            close(fd) ;
        }
        sleep(0) ;

        printf(1, "parent fork count : %d\n", getCount(SYS_fork));
        printf(1, "parent wait count : %d\n", getCount(SYS_wait));
        printf(1, "parent sleep count : %d\n" , getCount(SYS_sleep)) ;
        printf(1, "parent open count : %d\n" , getCount(SYS_open)) ;
        printf(1, "parent close count : %d\n" , getCount(SYS_close)) ;
        printf(1, "parent write count : %d\n", getCount(SYS_write));
        printf(1, "parent fstat count : %d\n", getCount(SYS_fstat));
        printf(1, "parent read count : %d\n", getCount(SYS_read));
        printf(1, "parent mknod count : %d\n", getCount(SYS_mknod));
    }
    exit();
}