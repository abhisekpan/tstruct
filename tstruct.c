
#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm/current.h>

/*
   static inline long long int read_gs(void)
   {
   long long int i;

   asm("\t movq %%gs, %0" : "=r"(i));
   return i;
   }
 */

static inline struct task_struct *try_current_task(unsigned long int p_offset)
{
	struct task_struct ** cur_task_maybe;
	unsigned long int offset;
	offset = p_offset;
	/* MSR_GS_BASE contains the base address of GS segment */
	cur_task_maybe = (struct task_struct **) 
		(native_read_msr(MSR_GS_BASE) + offset);
	if (*cur_task_maybe != NULL) {
		printk (KERN_INFO "value for %p is %p\n", cur_task_maybe, 
				*cur_task_maybe);
	}
	return (struct task_struct *) *cur_task_maybe;
}

static int __init tstruct_init(void)
{
	struct task_struct *cur_task;
	struct task_struct *cur_task_maybe;
	/*
	long long int gs_val;
	unsigned long int offset;
	*/
	unsigned int pid;
	/* processor id */
	pid = smp_processor_id();
	printk (KERN_INFO "processor id %u\n", pid);
	/*
	   for (offset = 0; offset < 0x02000; offset = offset + 0x040) {
	   cur_task_maybe = try_current_task(offset);
	   }
	 */
	cur_task = get_current();
	/* The offset of _per_cpu__current_task is 0Xcbc0 for kernel 2.6.32-38,
	 * ubuntu 10.04. This can be obtained by running:
	 * cat /proc/kallsyms | grep per_cpu__current_task
	 */
	cur_task_maybe = try_current_task(0x000000000000cbc0);
	if (cur_task == cur_task_maybe) {
		printk (KERN_INFO "success: offset is correct");
	} else {
		printk (KERN_INFO "failure: wrong offset");
	}
	printk(KERN_INFO "task is %p\n", cur_task);
	printk(KERN_INFO "task may be %p\n", cur_task_maybe);    

	printk(KERN_INFO "offsets: ts_state=%ld, ts_pid=%ld, ts_next=%ld, \
			ts_comm=%ld, ts_prev=%ld\n", 
			(long) offsetof(struct task_struct, state),
			(long) offsetof(struct task_struct, pid),
			(long) offsetof(struct task_struct, tasks.next),
			(long) offsetof(struct task_struct, comm),
			(long) offsetof(struct task_struct, tasks.prev));

	/*
	   gs_val = read_gs();
	   printk(KERN_INFO "gs_value is %lld\n", gs_val);
	 */
	return 0;
}

static void __exit tstruct_cleanup(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(tstruct_init);
module_exit(tstruct_cleanup);

