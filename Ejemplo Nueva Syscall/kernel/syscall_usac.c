#include <linux/syscalls.h>

SYSCALL_DEFINE0(usac_hello)
{
    printk(KERN_INFO "USAC syscall ejecutada\n");
    return 42;
}