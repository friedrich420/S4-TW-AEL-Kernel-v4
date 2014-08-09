#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x66ee3821, "module_layout" },
	{ 0x20c8bb79, "cdev_del" },
	{ 0xff328dc7, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xdc81a439, "cdev_init" },
	{ 0x15692c87, "param_ops_int" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0x50d2f6bd, "down_interruptible" },
	{ 0xd573109, "device_destroy" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x27e1a049, "printk" },
	{ 0xca0751f6, "device_create" },
	{ 0x1c3332b0, "cdev_add" },
	{ 0xbad8ccdf, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0xa72dab62, "up" },
	{ 0x563f8307, "class_destroy" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xe0715b4a, "__class_create" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

