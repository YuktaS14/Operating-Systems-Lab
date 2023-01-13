#include "user.h"
#include "memlayout.h"
#include "syscall.h"
#include "types.h"

int test4 = 12;

void getPAddr(int va)
{
    int pa = v2paddr(va);
    if(pa != -1)
    {
        printf(1,"Physical address for VA: 0x%x is 0x%x\n\n", va, pa);
    }
}
int main(int argc, char *argv[])
{
    int test1 = 112001052;
    char* test2 = "ALN";
    int test3 = KERNBASE+ 0x1009;

    printf(1,"Testing v2paddr() system call\n");

    getPAddr((int)&test1);
    getPAddr((int)&test2);
    getPAddr(test3);
    
    printf(1,"Global variable\n");
    getPAddr((int)&test4);
    
    printf(1,"\n\nCreating Child Proces \n\n");
    if(fork() == 0)
    {
        int test5 = test4;
        printf(1,"Parent's test1 variable in child Proces:\n");
        getPAddr((int)&test1);
        
        printf(1,"local variable in child:\n");
        getPAddr((int)test5);

    }
    else
    {
        wait();
        sleep(5);

        printf(1,"local variable(newly created) in parent Process:\n");

        int test5 = test4;
        getPAddr((int)test5);
    }
    exit();
}