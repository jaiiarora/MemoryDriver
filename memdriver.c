/*
 * A Memory Driver
 * NO Hardware Device Invoolved
 */

/*
 * Headers
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h> //for kmalloc()
#include <linux/uaccess.h> //copy_to or from_user()


#define mem_size 1024

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
uint8_t *kernel_buffer;

/*
 * Protoypes
 */

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
static int etx_open(struct inode *inode, struct file*  filp);
static int etx_release(struct inode *inode, struct file* filp);
static ssize_t etx_read(struct file* filp, char __user *buff, size_t count, loff_t* offp);
static ssize_t etx_write(struct file* filp,const char *buff, size_t len, loff_t *offp);

/*
 * Create File Operations Strucutre now
 */

static struct file_operations fops={
	.owner= THIS_MODULE,
	.read= etx_read,
	.write= etx_write,
	.open= etx_open,
	.release= etx_release,
};

/*
 * Called when the device file is opened via user space application
 */


static int etx_open(struct inode *inode, struct file*  filp)
{
	pr_info("You opened the Device File");
	return 0;
}


static int etx_release(struct inode *inode, struct file* filp)
{
	pr_info("You closed  the Device File");
	return 0;
}

/*
 * Reading from Device  Files
 */


static ssize_t etx_read(struct file* filp, char __user *buff, size_t count, loff_t* offp)
{
	//Copy data from Kernel Space  buffer to User Space Buffer
	if (copy_to_user(buff,  kernel_buffer, mem_size))
	{
		pr_err("Data Read: Err! \n");
	}
	/*
	 * If no error  exits with status 0
	 */

	pr_info("Data Read: Finished. \n");
	return mem_size;
}

/*
 * Writing to Device  FIles
 */


static ssize_t etx_write(struct file* filp, const char __user *buff, size_t len, loff_t *offp)
{
	//Copy data to  Kernel space
	if (copy_from_user(kernel_buffer, buff, len))
	{
		pr_err("Data Write: Err! \n");
	}
	pr_info("Write Successfull! \n");
	return len;
}


/*
 * Module INit Function
 */


static int __init etx_driver_init(void)
{
	/*
	 *  Allocate Major, Minor NUmbers
	 */

	if ((alloc_chrdev_region(&dev, 0, 1, "etx_dev"))<0)
	{
		pr_info("Cannot allocate major number \n");
		return -1;
	}
	pr_info("Major=%d Minor=%d \n", MAJOR(dev), MINOR(dev));
	pr_info("%d \n",dev);
	/*
	 * Create a Cdev structure
	 */

	cdev_init(&etx_cdev, &fops);
	
	/*
	 * Add character Device to the system */

	if ((cdev_add(&etx_cdev, dev,1))<0)
	{
		pr_info("Cannot Add Device to the System \n");
		goto r_class;
	}

	if ((dev_class=class_create(THIS_MODULE, "etx_class"))==NULL)
	{
		pr_info("Canot Create struct class \n");
		goto r_class;
	}

	/*Create Device*/
	if ((device_create(dev_class, NULL, dev, NULL, "etx_device"))== NULL)
	{
		pr_info("Cannot create the Device 1.\n ");
		goto r_device;
	}
	
	/*Create a Physical Memory*/

	if ((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0)
	{
		pr_info("Cannot allocate memory in kernel \n");
		goto r_device;
	}

	strcpy(kernel_buffer, "Hello_World");
	pr_info("Device Driver Inserted \n");
	return 0;

	r_device:
		class_destroy(dev_class);
	r_class:
		unregister_chrdev_region(dev,1);
		return -1;

}

/*
 * Module exit function
 */


static void __exit etx_driver_exit(void)
{
	kfree(kernel_buffer);
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev,1);
	pr_info("Removed Device Driver \n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jai Arora, jaiiarora");

















