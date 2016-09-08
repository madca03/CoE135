#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Agaton");

struct timer_list ntimer;

void timer_callback(unsigned long data)
{
	struct timeval current_time;
	long start_sec = (long) data;
	long current_sec;	
	int ret;

	do_gettimeofday(&current_time);
	current_sec = current_time.tv_sec;

	printk(KERN_INFO "elapsed seconds = %ld\n", current_sec - start_sec);
	ret = mod_timer(&ntimer, jiffies + msecs_to_jiffies(1000));
	if (ret) printk(KERN_INFO "Error in mod_timer\n");
}

static int __init mod_init(void)
{
	struct timeval start_time;
	int ret;
	
	do_gettimeofday(&start_time);
	setup_timer(&ntimer, timer_callback, start_time.tv_sec);
	ret = mod_timer(&ntimer, jiffies + msecs_to_jiffies(1000));
	if (ret) printk(KERN_INFO "Error in mod_timer\n");
	
	return 0;
}

static void __exit mod_exit(void)
{
	del_timer(&ntimer);
	printk(KERN_INFO "Cleaning up module\n");
}

module_init(mod_init);
module_exit(mod_exit);
