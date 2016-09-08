#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Agaton");

static int hz_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", HZ);
	return 0;
}

static int hz_open(struct inode *inode, struct file *file)
{
	return single_open(file, hz_show, NULL);
}

static const struct file_operations hz_fops = {
	.owner = THIS_MODULE,
	.open = hz_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init mod_init(void)
{
	printk(KERN_INFO "Loading hz module, HZ = %d.\n", HZ);
	proc_create("hz", 0, NULL, &hz_fops);
	return 0;
}

static void __exit mod_exit(void)
{
	remove_proc_entry("hz", NULL);
	printk(KERN_INFO "Unloading the module\n");
}

module_init(mod_init);
module_exit(mod_exit);
