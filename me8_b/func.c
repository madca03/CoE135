#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
//#include <asm/mc146818rtc.h>
#include <asm/rtc.h>
#include <linux/rtc.h>

MODULE_LICENSE("GPL");

static int __init mod_init(void)
{
	struct rtc_time time;
	unsigned long sec_time;
	int offset;
	struct tm result;
	get_rtc_time(&time);
	rtc_tm_to_time(&time, &sec_time);

	if (sys_tz.tz_minuteswest < 0) offset = (sys_tz.tz_minuteswest * (-1)) * 60;
	else offset = sys_tz.tz_minuteswest * 60;
	time_to_tm(sec_time, offset, &result); 
	printk(KERN_INFO "%d:%d:%d %d/%d/%ld\n", result.tm_hour, result.tm_min, result.tm_sec, result.tm_mon, result.tm_mday, result.tm_year + 1900);

	return 0;
}

static void __exit mod_exit(void)
{

}

module_init(mod_init);
module_exit(mod_exit);
