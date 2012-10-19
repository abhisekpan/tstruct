
#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

//#include <stddef.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <./mysched.h>
//#include <asm-generic/current.h>
//#include <./tstruct.h>

#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>        // included for __init and __exit macros
#include <linux/sched.h>
static int __init tstruct_init(void)
{
    struct task_struct *cur_task;
    cur_task = get_current();
    printk(KERN_INFO "offsets: ts_state=%ld, ts_pid=%ld, ts_next=%ld ts_comm=%ld, ts_prev=%ld\n", 
            (long) offsetof(struct task_struct, state),
            (long) offsetof(struct task_struct, pid),
            (long) offsetof(struct task_struct, tasks.next),
            (long) offsetof(struct task_struct, comm),
            (long) offsetof(struct task_struct, tasks.prev));
    printk(KERN_INFO "task is %p\n", cur_task);
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit tstruct_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(tstruct_init);
module_exit(tstruct_cleanup);



//struct task_struct;
/*int main(void)
{
    //struct task_struct *cur_task;
    //cur_task = get_current();


    printf("offsets: i=%ld; c=%ld; d=%ld a=%ld\n",
            (long) offsetof(struct task_struct, pid),
            (long) offsetof(struct s, c),
            (long) offsetof(struct s, d),
            (long) offsetof(struct s, a));
    printf("sizeof(struct s)=%ld\n", (long) sizeof(struct s));

    exit(EXIT_SUCCESS);
}*/

