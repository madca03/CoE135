#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Agaton");

#define SECS_PER_HOUR (60 * 60)

enum dayofweek {
	Sun,
	Mon,
	Tue,
	Wed,
	Thu,
	Fri,
	Sat
};

enum month {
	Jan,
	Feb,
	Mar,
	Apr,
	May,
	Jun,
	Jul,
	Aug,
	Sep,
	Oct,
	Nov,
	Dec
};

char *get_dayofweek(enum dayofweek num)
{
	switch(num) {
		case Sun:
			return "Sun";
		case Mon:
			return "Mon";
		case Tue:
			return "Tue";
		case Wed:
			return "Wed";
		case Thu:
			return "Thu";
		case Fri:
			return "Fri";
		case Sat:
			return "Sat";
		default:
			return NULL;
	}
}

char *get_month(enum month num)
{
	switch(num)	{
		case Jan:
			return "Jan";
		case Feb:
			return "Feb";
		case Mar:
			return "Mar";
		case Apr:
			return "Apr";
		case May:
			return "May";
		case Jun:
			return "Jun";
		case Jul:
			return "Jul";
		case Aug:
			return "Aug";
		case Sep:
			return "Sep";
		case Oct:
			return "Oct";
		case Nov:
			return "Nov";
		case Dec:
			return "Dec";
		default:
			return NULL;
	}
}

char *get_hour(int hour)
{
	char *c_hour;

	c_hour = kmalloc(3, GFP_KERNEL);

	if (hour < 10) sprintf(c_hour, "0%d", hour);
	else sprintf(c_hour, "%d", hour);

	return c_hour;
}

char *get_min(int min)
{
	char *c_min;

	c_min = kmalloc(3, GFP_KERNEL);

	if (min < 10) sprintf(c_min, "0%d", min);
	else sprintf(c_min, "%d", min);

	return c_min;
}

char *get_sec(int sec)
{
	char *c_sec;

	c_sec = kmalloc(3, GFP_KERNEL);

	if (sec < 10) sprintf(c_sec, "0%d", sec);
	else sprintf(c_sec, "%d", sec);

	return c_sec;
}

long get_year(int num)
{
	return 1900 + num;
}

static int __init mod_init(void)
{
	struct timeval ktv;
	struct tm ktm;	

	// struct timezone sys_tz contains the information
	// about the timezone

	do_gettimeofday(&ktv);
	time_to_tm(ktv.tv_sec, 8 * SECS_PER_HOUR, &ktm);

	printk(KERN_INFO "%s %s %d %s:%s:%s PHT %ld\n", 
		get_dayofweek((enum dayofweek) ktm.tm_wday),
		get_month((enum month) ktm.tm_mon),
		ktm.tm_mday,
		get_hour(ktm.tm_hour),
		get_min(ktm.tm_min),
		get_sec(ktm.tm_sec),
		get_year(ktm.tm_year));
	return 0;
}

static void __exit mod_exit(void)
{
}

module_init(mod_init);
module_exit(mod_exit);
