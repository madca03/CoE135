#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

void DFS(struct task_struct *task)
{
	struct task_struct *child;
	struct list_head *list;

	printk("name: %s, pid: [%d], state: %li\n", task->comm, task->pid, task->state);
	list_for_each(list, &task->children) {
		child = list_entry(list, struct task_struct, sibling);
		DFS(child);
	}
}

static int __init mod_init(void)
{
	struct task_struct *p;
	/*
	for_each_process(p) {
		printk(KERN_INFO "comm = %s pid = %d\n", p->comm, task_pid_nr(p));
	}*/
	DFS(&init_task);
	return 0;
}

static void __exit mod_exit(void)
{

}

module_init(mod_init);
module_exit(mod_exit);
