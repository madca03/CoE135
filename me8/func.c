#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/rtc.h>
#include <linux/timekeeping.h>
#include <linux/mc146818rtc.h>
#include <linux/spinlock.h>
#include <linux/time.h>
#include <linux/bcd.h>
#include <asm/rtc.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Agaton");

#define CMOS_YEARS_OFFS 2000

static int __init mod_init(void)
{
	struct timespec now;

	// access timezone at struct timezone sys_tz in "linux/time.h"

    // from "/arch/x86/kernel/rtc.c" -> "mach_get_cmos_time"
	unsigned int status, year, mon, day, hour, min, sec, century = 0;
	unsigned long flags;
	char c_hour[3], c_min[3], c_sec[3];
	char c_mon[3], c_day[3];	
	struct tm result;
	int offset;

	while (1) {
		if (!rtc_is_updating()) {
			spin_lock_irqsave(&rtc_lock, flags);
			
			sec = CMOS_READ(RTC_SECONDS);
			min = CMOS_READ(RTC_MINUTES);
			hour = CMOS_READ(RTC_HOURS);
			day = CMOS_READ(RTC_DAY_OF_MONTH);
			mon = CMOS_READ(RTC_MONTH);
			year = CMOS_READ(RTC_YEAR);
			status = CMOS_READ(RTC_CONTROL);	

			spin_unlock_irqrestore(&rtc_lock, flags);
			break;
		}
	}

	if (RTC_ALWAYS_BCD || !(status & RTC_DM_BINARY)) {
		sec = bcd2bin(sec);
		min = bcd2bin(min);
		hour = bcd2bin(hour);
		day = bcd2bin(day);
		mon = bcd2bin(mon);
		year = bcd2bin(year);
	}

	if (century) {
		century = bcd2bin(century);
		year += century * 100;
	} else
		year += CMOS_YEARS_OFFS;

	now.tv_sec = mktime(year, mon, day, hour, min, sec);
	now.tv_nsec = 0;

	if (sys_tz.tz_minuteswest < 0) offset = (sys_tz.tz_minuteswest * (-1)) * 60;
	else offset = sys_tz.tz_minuteswest * 60;
	
	printk(KERN_INFO "offset = %d\n", offset);

	time_to_tm(now.tv_sec, offset, &result);

	if (hour < 10) sprintf(c_hour, "0%d", hour);
	else sprintf(c_hour, "%d", hour);
		
	if (min < 10) sprintf(c_min, "0%d", min);
	else sprintf(c_min, "%d", min);

	if (sec < 10) sprintf(c_sec, "0%d", sec);
	else sprintf(c_sec, "%d", sec);

	if (mon < 10) sprintf(c_mon, "0%d", mon);
	else sprintf(c_mon, "%d", mon);

	if (day < 10) sprintf(c_day, "0%d", day);
	else sprintf(c_day, "%d", day);
	
	printk(KERN_INFO "%s:%s:%s %s/%s/%u\n", c_hour, c_min, c_sec, c_mon, c_day, year);
	printk(KERN_INFO "tz_minuteswest = %d\n", sys_tz.tz_minuteswest);
	printk(KERN_INFO "tz_dsttime = %d\n", sys_tz.tz_dsttime);

	printk(KERN_INFO "%d:%d:%d %d/%d/%ld\n", result.tm_hour, result.tm_min, result.tm_sec, result.tm_mon, result.tm_mday, result.tm_year);

	return 0;
}

static void __exit mod_exit(void)
{

}

module_init(mod_init);
module_exit(mod_exit);
