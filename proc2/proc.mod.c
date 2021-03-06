#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x884142c7, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x2990a4c8, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0xad9dd64, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x281f15d6, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x9108e9ec, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x3488723d, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xdad4037b, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0x7f554878, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4645DC642178F51C323BF13");
