/* ofd.c our first linux driver program */
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

static dev_t first; // Global variable for the first device number

static int __init ofd_init(void) /* constructor */
{
	printk(KERN_INFO "Hello: ofd registered");
	if(alloc_chrdev_region(&first, 0, 3, "anup") < 0){
		return -1;
	}
	printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));
	return 0;
}

static void __exit ofd_exit(void) /* Destructor */
{
	printk(KERN_INFO "Bye: ofd deregistered");
	unregister_chrdev_region(first, 3);
}

module_init(ofd_init);
module_exit(ofd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anup Warnulkar");
MODULE_DESCRIPTION("My first driver");

