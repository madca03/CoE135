#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Agaton");

static int __init mod_init(void)
{
	void *ptr;
	unsigned long base_phys_addr;
	unsigned long int i; 

	printk(KERN_INFO "allocating 1 byte\n");
	ptr = kmalloc(1, GFP_KERNEL);
	if (ptr == NULL) {
		printk(KERN_INFO "Error in memory allocation\n");
	}

	printk(KERN_INFO "size = %u\n", ksize(ptr));
	kfree(ptr);

	base_phys_addr = virt_to_phys(ptr);
	
	for (i = 0; i < ksize(ptr); i++) {
		if ( (base_phys_addr + i) != virt_to_phys(ptr + i) ) {
			printk(KERN_INFO "kmalloc is not contiguous\n");
			break;
		}
	}

	printk(KERN_INFO "kmalloc is contiguous\n");
/*	
	printk(KERN_INFO "page size = %lu\n", PAGE_SIZE);
	printk(KERN_INFO "max cache = %lu\n", KMALLOC_MAX_CACHE_SIZE);
	printk(KERN_INFO "max shift high = %d\n", KMALLOC_SHIFT_HIGH);
*/
	i = (PAGE_SIZE/4) * PAGE_SIZE;
	while (1) {
		ptr = kmalloc(i, GFP_KERNEL);
		if (ptr == NULL) {
			printk(KERN_INFO "memory allocation failed at %lu bytes\n", i);
			break;
		}
		kfree(ptr);
		printk(KERN_INFO "memory allocation success at %lu\n", i);
		i++;
	}

	return 0;
}

static void __exit mod_exit(void)
{
	
}

module_init(mod_init);
module_exit(mod_exit);
