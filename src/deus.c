
#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/ioctl.h>
#include "deus.h"

#include "driver.h"


#include <linux/uaccess.h>          // Required for the copy to user function
#define  DEVICE_NAME "deus-ctrl"
#define  CLASS_NAME  "deus"

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Lambda Lab");
MODULE_DESCRIPTION("Deus - Devices existing in user space");
MODULE_VERSION("0.1");              ///< The version of the module

static int    majorNumber;                  ///< Stores the device number -- determined automatically
static struct class*  deuscharClass  = NULL; ///< The device-driver class struct pointer
static struct device* deuscharDevice = NULL; ///< The device-driver device struct pointer

// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
static long     dev_ioctl(struct file *, unsigned int, unsigned long);

/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
   .unlocked_ioctl = dev_ioctl
};




/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init deus_init(void){
   printk(KERN_INFO "deus: Starting module\n");
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "deusChar failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "deusChar: registered correctly with major number %d\n", majorNumber);

   // Register the device class
   deuscharClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(deuscharClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(deuscharClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "deusChar: device class registered correctly\n");

   // Register the device driver
   deuscharDevice = device_create(deuscharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(deuscharDevice)){               // Clean up if there is an error
      class_destroy(deuscharClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(deuscharDevice);
   }
   printk(KERN_INFO "deusChar: device class created correctly\n"); // Made it! device was initialized
   driver_prepare();
   return 0;
}

/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit deus_exit(void){
   device_destroy(deuscharClass, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(deuscharClass);                          // unregister the device class
   class_destroy(deuscharClass);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
}


static int dev_open(struct inode *inodep, struct file *filep){
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   return 0;
}

static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "deusChar: Device successfully closed\n");
   return 0;
}


static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
	switch (cmd) {
		case IOCTL_SEND_EVENT_RESULT:
			
			return 0;
		case IOCTL_REGISTER_MASTER:
         
			return 0;
		case IOCTL_MASTER_WAIT_EVENT:
			return 0;
		default:
			return -EINVAL;
	}
}


/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(deus_init);
module_exit(deus_exit);
