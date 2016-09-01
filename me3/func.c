#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/sort.h>

#define NMEMB (10)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Agaton");

int compare_asc(const void *a, const void *b)
{
	// if a > b then a - b > 0
	// if a < b then a - b < 0
	// if a == b then a - b = 0
	return (*(int *)a - *(int *)b);
}

int compare_desc(const void *a, const void *b)
{
	return (*(int *)b - *(int *)a);
}

static int __init mod_init(void)
{
	char *string1;
	char string2[] = "Hello World";
	char *string3;
	char string4[] = "Mark Agaton";
	int ret_cmp;
	char c = 'W';
	char *ret_chr;
	u32 rand_num;
	int rand_arr[NMEMB];
	int i;

	// ssize_t strscpy(char *dest, const char *src, size_t count)
	string1 = kmalloc(strlen(string2) + 1, GFP_KERNEL);
	if (strscpy(string1, (const char *)string2, strlen(string2) + 1) == -E2BIG) {
		printk(KERN_INFO "Destination buffer wasn't big enough\n");
	}
	string3 = kmalloc(strlen(string4) + 1, GFP_KERNEL);
	if (strscpy(string3, (const char *)string4, strlen(string4) + 1) == -E2BIG) {
		printk(KERN_INFO "Destination buffer wasn't big enough\n");
	}

	printk(KERN_INFO "string%d = %s\n", 1, string1);
	printk(KERN_INFO "string%d = %s\n", 3, string3);
	
	// int strncmp(const char *cs, const char *ct, size_t count)
	ret_cmp = strncmp(string1, string3, strlen(string3));
	printk(KERN_INFO "%s compared to %s -> %s\n", 
		string1, string3, ret_cmp == -1 ? "right larger" : ret_cmp == 1 ? "left larger" : "equal");
	ret_cmp = strncmp(string3, string1, strlen(string3));
	printk(KERN_INFO "%s compared to %s -> %s\n", 
		string3, string1, ret_cmp == -1 ? "right larger" : ret_cmp == 1 ? "left larger" : "equal");
	ret_cmp = strncmp(string1, string2, strlen(string2));
	printk(KERN_INFO "%s compared to %s -> %s\n", 
		string1, string2, ret_cmp == -1 ? "right larger" : ret_cmp == 1 ? "left larger" : "equal");
	

	// char *strchr(const char *s, int c)
	ret_chr = strchr(string1, c);
	printk(KERN_INFO "'%c' in string '%s' -> %s\n", c, string1, ret_chr == NULL ? "No" : "Yes");
	ret_chr = strchr(string3, c);
	printk(KERN_INFO "'%c' in string '%s' -> %s\n", c, string3, ret_chr == NULL ? "No" : "Yes");

	// u32 prandom_u32(void)
	rand_num = prandom_u32();
	printk(KERN_INFO "rand_num = %u\n", rand_num);
	rand_num = prandom_u32();
	printk(KERN_INFO "rand_num = %u\n", rand_num);

	/* void sort(void *base, size_t num, size_t size,
		int (*cmp_func)(const void *, const void *),
		void (*swap_func)(void *, void *, int size))
	*/
	
	for (i = 0; i < sizeof(rand_arr)/sizeof(rand_arr[0]); i++) {
		rand_arr[i] = (int)(prandom_u32() % 100);
	}

	printk(KERN_INFO "unsorted array:\n");
	for (i = 0; i < NMEMB; i++) {
		printk(KERN_INFO "%d\n", rand_arr[i]);
	}

	sort((void *)rand_arr, sizeof(rand_arr)/sizeof(rand_arr[0]), sizeof(rand_arr[0]), compare_asc, NULL);

	printk(KERN_INFO "sorted array (ascending):\n");
	for (i = 0; i < NMEMB; i++) {
		printk(KERN_INFO "%d\n", rand_arr[i]);
	}

	sort((void *)rand_arr, sizeof(rand_arr)/sizeof(rand_arr[0]), sizeof(rand_arr[0]), compare_desc, NULL);

	printk(KERN_INFO "sorted array (descending):\n");
	for (i = 0; i < NMEMB; i++) {
		printk(KERN_INFO "%d\n", rand_arr[i]);
	}

	kfree(string1);
	kfree(string3);
	return 0;
}

static void __exit mod_exit(void)
{
	
}

module_init(mod_init);
module_exit(mod_exit);
