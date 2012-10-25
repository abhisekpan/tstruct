
#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>        // included for __init and __exit macros
#include <linux/sched.h>
#include <asm/current.h>
//DECLARE_PER_CPU(struct task_struct *, current_task);

static inline long long int read_gs(void)
{
    long long int i;

    asm("\t movq %%gs, %0" : "=r"(i));
    return i;
}

static inline struct task_struct *try_current_task(void)
{
    unsigned long long cur_task_maybe;
    unsigned int offset;
    offset = 0xb000;
    asm("\t movq %%gs:%P1, %0" : "=r"(cur_task_maybe) : "m"(offset));
    //cur_task_maybe = (struct task_struct *)(native_read_msr(MSR_GS_BASE) + 0xb000);
    printk (KERN_INFO "value is %llu\n", cur_task_maybe);
    return (struct task_struct *) cur_task_maybe;
}


static int __init tstruct_init(void)
{
    struct task_struct *cur_task;
    struct task_struct *cur_task_maybe;
    int id;
    long long int gs_val;
    cur_task = get_current();
    cur_task_maybe = try_current_task();
    if (cur_task == cur_task_maybe) {
	printk (KERN_INFO "success");
    } else {
	printk (KERN_INFO "failure");
    }
    printk(KERN_INFO "offsets: ts_state=%ld, ts_pid=%ld, ts_next=%ld ts_comm=%ld, ts_prev=%ld\n", 
            (long) offsetof(struct task_struct, state),
            (long) offsetof(struct task_struct, pid),
            (long) offsetof(struct task_struct, tasks.next),
            (long) offsetof(struct task_struct, comm),
            (long) offsetof(struct task_struct, tasks.prev));
    
    printk(KERN_INFO "task is %p\n", cur_task);
    printk(KERN_INFO "task may be %p\n", cur_task_maybe);    

    id = __my_cpu_offset;
    printk(KERN_INFO "cpu offset is %d\n", id);
    //printk(KERN_INFO "offset is %p\n", per_cpu_var(current_task));

    gs_val = read_gs();
    printk(KERN_INFO "gs_value is %lld\n", gs_val);
    
    return 0;
}

static void __exit tstruct_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(tstruct_init);
module_exit(tstruct_cleanup);

