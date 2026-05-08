#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Hello World kernel module with parameter");

// Module parameter
static char *name = "Master";

module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "Does something with this name");

// Init function
static int __init hello_init(void) {
    printk(KERN_INFO "Hello %s from the kernel!\n", name);
    return 0;
}

// Exit function
static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye %s from the kernel!\n", name);
}

module_init(hello_init);
module_exit(hello_exit);