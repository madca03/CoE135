#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Agaton");

int init_prio = 0;
struct task_struct *init_proc;

static int __init mod_init(void)
{
	init_proc = next_task(&init_task);
	init_prio = init_proc->prio;

	printk(KERN_INFO "comm = %s\n", init_proc->comm);
	printk(KERN_INFO "priority level = %d\n", init_proc->prio);
	printk(KERN_INFO "policy = ");

	switch (init_proc->policy) {
		case SCHED_NORMAL:
			printk(KERN_INFO "SCHED_NORMAL\n");
			break;
		case SCHED_FIFO:
			printk(KERN_INFO "SCHED_FIFO\n");
			break;
		case SCHED_RR:
			printk(KERN_INFO "SCHED_RR\n");
			break;
		case SCHED_BATCH:
			printk(KERN_INFO "SCHED_BATCH\n");
			break;
		case SCHED_IDLE:
			printk(KERN_INFO "SCHED_IDLE\n");
			break;
		case SCHED_DEADLINE:
			printk(KERN_INFO "SCHED_DEADLINE\n");
			break;
		case SCHED_RESET_ON_FORK:
			printk(KERN_INFO "SCHED_RESET_ON_FORK\n");
			break;
	}

	printk(KERN_INFO "Changing priority of init process...\n");
	set_user_nice(init_proc, -20);
//	init_proc->prio = 99;
	printk(KERN_INFO "priority level = %d\n", init_proc->prio);	
	return 0;
}

static void __exit mod_exit(void)
{
	printk(KERN_INFO "Returning the original priority of init process...\n");
	set_user_nice(init_proc, 0);
	printk(KERN_INFO "priority level = %d\n", init_proc->prio);
}

module_init(mod_init);
module_exit(mod_exit);
