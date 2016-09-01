#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Agaton");

#define SECS_PER_MIN (60)
#define SECS_PER_HOUR (60 * 60)

// from fs/proc/loadavg.c
#define LOAD_INT(x) ((x) >> FSHIFT)
#define LOAD_FRAC(x) LOAD_INT(((x) & (FIXED_1-1)) * 100)

struct uid {
	unsigned int val;
	struct list_head list;
};

// from kernel/sched/loadavg.c
void get_avenrun(unsigned long *loads, unsigned long offset, int shift)
{
	loads[0] = (avenrun[0] + offset) << shift;
	loads[1] = (avenrun[1] + offset) << shift;
	loads[2] = (avenrun[2] + offset) << shift;
}

void get_time(struct tm *ktm)
{
	struct timeval ktv;

	do_gettimeofday(&ktv);
	time_to_tm(ktv.tv_sec, 20 * SECS_PER_HOUR, ktm);
}

void get_uptime(int* uptime_hour, int* uptime_min)
{
	struct timespec uptime;
	get_monotonic_boottime(&uptime);

	*uptime_hour = uptime.tv_sec / SECS_PER_HOUR;
	*uptime_min = (uptime.tv_sec / SECS_PER_MIN) % SECS_PER_MIN;
}

int is_unique_uid(unsigned int pos_uid, struct list_head *uid_list)
{
	struct uid *pos;
	
	list_for_each_entry(pos, uid_list, list) {
		if (pos->val == pos_uid)
			return 0;
	}

	return 1;
}



static int __init mod_init(void)
{
	struct tm ktm;	
	struct task_struct *pos;
	struct uid *uid_pos;
	struct uid *new_uid;
	int uidcount = 0;
	int uptime_hour;
	int uptime_min;
	unsigned long avnrun[3];
	unsigned int pos_uid;
	
	
	LIST_HEAD(uid_list);

	get_time(&ktm);
	get_uptime(&uptime_hour, &uptime_min);
	get_avenrun(avnrun, FIXED_1/200, 0);

	for_each_process(pos) {
		// task_uid() returns pos->real_cred->uid of type struct kuid_t
		// __kuid_val() returns kuid_t.val of type unsigned int
		
		pos_uid = __kuid_val(task_uid(pos));

		if (pos_uid >= 1000 && pos_uid <= 60000 && is_unique_uid(pos_uid, &uid_list)) {
			new_uid = kmalloc(sizeof(struct uid), GFP_KERNEL);
			new_uid->val = pos_uid;
			INIT_LIST_HEAD(&new_uid->list);

			list_add(&new_uid->list, &uid_list);
		}
	}

	list_for_each_entry(uid_pos, &uid_list, list) {
		uidcount++;
	}
	
	if (!uptime_hour) {
		printk(KERN_INFO " %d:%d:%d up %d min, %d user, load average: %lu.%02lu %lu.%02lu %lu.%02lu\n", 
			ktm.tm_hour, ktm.tm_min, ktm.tm_sec,
			uptime_min, uidcount,
			LOAD_INT(avnrun[0]), LOAD_FRAC(avnrun[0]),
			LOAD_INT(avnrun[1]), LOAD_FRAC(avnrun[1]),
			LOAD_INT(avnrun[2]), LOAD_FRAC(avnrun[2]));
	} else {
		if (uptime_min < 10) {
			printk(KERN_INFO " %d:%d:%d up %d:0%d, %d user, load average: %lu.%02lu %lu.%02lu %lu.%02lu\n", 
				ktm.tm_hour, ktm.tm_min, ktm.tm_sec,
				uptime_hour, uptime_min, uidcount,
				LOAD_INT(avnrun[0]), LOAD_FRAC(avnrun[0]),
				LOAD_INT(avnrun[1]), LOAD_FRAC(avnrun[1]),
				LOAD_INT(avnrun[2]), LOAD_FRAC(avnrun[2]));
		} else {
			printk(KERN_INFO " %d:%d:%d up %d:%d, %d user, load average: %lu.%02lu %lu.%02lu %lu.%02lu\n", 
				ktm.tm_hour, ktm.tm_min, ktm.tm_sec,
				uptime_hour, uptime_min, uidcount,
				LOAD_INT(avnrun[0]), LOAD_FRAC(avnrun[0]),
				LOAD_INT(avnrun[1]), LOAD_FRAC(avnrun[1]),
				LOAD_INT(avnrun[2]), LOAD_FRAC(avnrun[2]));
		}
		
	}

	
	return 0;
}

static void __exit mod_exit(void)
{
	
}

module_init(mod_init);
module_exit(mod_exit);
