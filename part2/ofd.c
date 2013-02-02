/* ofd.c our first linux driver program */
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

static int __init ofd_init(void) /* constructor */
{
	printk(KERN_INFO "Hello: ofd registered");
	return 0;
}

static void __exit ofd_exit(void) /* Destructor */
{
	printk(KERN_INFO "Bye: ofd deregistered");
}

module_init(ofd_init);
module_exit(ofd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anup Warnulkar");
MODULE_DESCRIPTION("My first driver");

