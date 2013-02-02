/* ofd.c our first linux driver program */
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for character device structure
static struct class *c1; // Global variable for device class
static char c;

static int my_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Driver: open()\n");
	return 0;
}

static int my_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO  "Driver: close()\n");
	return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Driver: read()\n");
	if(copy_to_user(buf, &c, 1) != 0)
		return -EFAULT;
	else
		return 1;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Driver: write()\n");
	if(copy_from_user(&c, buf + len -1, 1) != 0)
		return -EFAULT;
	else
		return len;
}

static struct file_operations anup_fops =
{
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.read = my_read,
	.write = my_write
};





static int __init ofd_init(void) /* constructor */
{
	printk(KERN_INFO "Hello: ofd registered");
	if(alloc_chrdev_region(&first, 0, 1, "anup") < 0){
		return -1;
	}
	if((c1 = class_create(THIS_MODULE, "chardrv")) == NULL)
	{
		unregister_chrdev_region(first, 1);
		return -1;
	}
	if(device_create(c1, NULL, first, NULL, "mynull") == NULL)
	{
		class_destroy(c1);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	cdev_init(&c_dev, &anup_fops);
	if(cdev_add(&c_dev, first, 1) == -1)
	{
		device_destroy(c1, first);
		class_destroy(c1);
		unregister_chrdev_region(first, 1);
	}

	printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));
	return 0;
}

static void __exit ofd_exit(void) /* Destructor */
{
	printk(KERN_INFO "Bye: ofd deregistered");
	cdev_del(&c_dev);
	device_destroy(c1, first);
	class_destroy(c1);
	unregister_chrdev_region(first, 1);
}

module_init(ofd_init);
module_exit(ofd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anup Warnulkar");
MODULE_DESCRIPTION("My first driver");

