#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Agaton");

static int jiffies_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%llu\n", (unsigned long long) get_jiffies_64());
	return 0;
}

static int jiffies_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, jiffies_proc_show, NULL);
}

static const struct file_operations jiffies_proc_fops = {
	.owner = THIS_MODULE,
	.open = jiffies_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release
};

static int __init mod_init(void)
{
	proc_create("crash_jiffies", 0, NULL, &jiffies_proc_fops);
	return 0;
}

static void __exit mod_exit(void)
{
	remove_proc_entry("crash_jiffies", NULL);
}

module_init(mod_init);
module_exit(mod_exit);
