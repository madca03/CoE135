#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Agaton");

struct task_struct *task_of(struct sched_entity *se)
{
	return container_of(se, struct task_struct, se);
}

static int __init mod_init(void)
{
	struct rb_node *rson;
	struct rb_node *lson;
	struct sched_entity *se_parent;
	struct rb_node *parent2;
	struct task_struct *task_parent;

	printk(KERN_INFO "priority level = %d\n", current->prio);

	lson = (current->se).run_node.rb_left;
	rson = (current->se).run_node.rb_right;
	se_parent = (current->se).parent;
	parent2 = &(se_parent->run_node);

	if (lson == NULL) {
		printk(KERN_INFO "there is no left son\n");
	} else {
		printk(KERN_INFO "address lson = %p\n", lson);
	}

	if (rson == NULL) {
		printk(KERN_INFO "there is no right son\n");
	} else {
		printk(KERN_INFO "address rson = %p\n", rson);
	}
	
	if (parent2 == NULL) {
		printk(KERN_INFO "parent2 is NULL\n");
	} else {
		printk(KERN_INFO "address parent2 = %p\n", parent2);
	}

	task_parent = task_of(se_parent);
	printk(KERN_INFO "task_parent: %s\n", task_parent->comm);
	return 0;
}

static void __exit mod_exit(void)
{
	printk(KERN_INFO "Cleaning up the module\n");
}

module_init(mod_init);
module_exit(mod_exit);
