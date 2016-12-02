#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>
#include <asm/uaccess.h>

/*
 *helper function defined in project 
 */
unsigned char get_random_byte(int max){
	unsigned char c;
	get_random_bytes(&c, 1);
	return c%max;
}
/*
 * dice_read is the function called when a process calls read() on
 * /dev/dice.
 */
static ssize_t dice_read(struct file * file, char * buf, size_t count, loff_t *ppos){
	//getting roll
	char dice;
	dice = (char)get_random_byte(6);
	dice = dice + 1;
	//passing data to user
	if (copy_to_user(buf, &dice, (int)1)){
		return -EINVAL;
	}

	/*
	 * Tell the user how much data we wrote.
	 */
	*ppos = count;
	return count;
}

/*
 * The only file operation we care about is read.
 */
static const struct file_operations dice_fops = {
	.owner		= THIS_MODULE,
	.read		= dice_read,
};

static struct miscdevice dice_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/dice.
	 */
	"dice_driver",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&dice_fops
};

static int __init dice_init(void){

	int ret;
	/*
	 * Create the "dice" device in the /sys/class/misc directorynano.
	 * Udev will automatically create the /dev/dice device using
	 * the default rules.
	 */
	ret = misc_register(&dice_dev);
	if (ret){
		printk(KERN_ERR "Unable to register dice driver misc device\n");
	}
	return ret;
}

module_init(dice_init);

static void __exit dice_exit(void){
	misc_deregister(&dice_dev);
}

module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nathaniel Todd");
MODULE_DESCRIPTION("dice driver");
MODULE_VERSION("dev");
