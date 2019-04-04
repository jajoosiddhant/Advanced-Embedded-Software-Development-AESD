
/**
 * @file i2c_module.c
 * @author Siddhant Jajoo and Satya Mehta
 * @brief Kernel driver for I2C
 * @version 0.1
 * @date 2019-03-30
 * 
 * @references: http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
 * https://www.kernel.org/doc/html/v4.14/driver-api/i2c.html
 * http://renjucnair.blogspot.com/2012/01/writing-i2c-driver.html
 * https://github.com/torvalds/linux/blob/master/include/linux/i2c.h
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "myi2c_char"
#define CLASS_NAME "myclass"

static int my_majornumber;
static struct class *myclass = NULL;   // The device-driver class struct pointer
static struct device *mydevice = NULL; // The device-driver device struct pointer

/*Prototype functions for character device*/
static int my_open(struct inode *, struct file *);
static int my_close(struct inode *, struct file *);
static ssize_t my_read(struct file *, char *, size_t, loff_t *);
static ssize_t my_write(struct file *, const char *, size_t, loff_t *);
static long my_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);

static struct file_operations my_fileop = {
    .open = my_open,
    .write = my_write,
    .read = my_read,
    .release = my_close,
    .unlocked_ioctl = my_ioctl,
};

// static struct i2c_adapter my_adapter = {
//     .owner = THIS_MODULE,
//     //.class = I2C_ADAP_CLASS_SMBUS,
//     //.algo = &my_algo,
//     .name = "myi2c_adapter",
// };

// static struct i2c_algorithm my_algo =
// {
//.master_xfer = my_xfer_function,
//.functionality = my_function,
//.smbus_xfer = my_access_func,
// };

//static struct i2c_driver my_driver =
//{

//};

/**
 * @brief The kernel module initialization function. The __init macro means that for a built-in driver 
 * (not for a linux kernel module), the function is only used at initialization time and it can be
 * discarded and its memory freed after that. The static keyword restricts its visibility within this file.
 * 
 * @return int myi2c_init 
 */
static int __init myi2c_init(void)
{
    //int res;
    printk(KERN_ALERT "Entered I2C Module.\n");
    printk(KERN_ALERT "\n");

    my_majornumber = register_chrdev(0, DEVICE_NAME, &my_fileop);
    if (my_majornumber < 0)
    {
        printk(KERN_ALERT "Failed to register Major number.\n");
        return my_majornumber;
    }
    else
    {
        printk(KERN_ALERT "Successfully registered with Major number %d.\n", my_majornumber);
    }

    // Registering the device class
    myclass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(myclass))
    { // Check for error and clean up if there is
        unregister_chrdev(my_majornumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(myclass); // Correct way to return an error on a pointer
    }
    else
    {
        printk(KERN_INFO "Device class registered correctly\n");
    }

    // Register the device driver
    mydevice = device_create(myclass, NULL, MKDEV(my_majornumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(mydevice))
    {                           // Clean up if there is an error
        class_destroy(myclass); // Repeated code but the alternative is goto statements
        unregister_chrdev(my_majornumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(mydevice);
    }
    else
    {
        printk(KERN_INFO "Device class created correctly\n"); // Made it! device was initialized
    }

    // res = i2c_add_adapter(&my_adapter);
    // if (res)
    // {
    //     return res;
    // }
    return 0;
}

/**
 * @brief The kernel module exit function The __exit macro signifies that if this code is used for a
 * built-in driver (not a linux kernel module) , then this function is not required.
 * This function is similar to the iniialization function.
 * 
 */
static void __exit myi2c_exit(void)
{
    printk(KERN_ALERT "Exit I2C Module.\n");
    printk(KERN_ALERT "\n");

    device_destroy(myclass, MKDEV(my_majornumber, 0)); // remove the device
    class_unregister(myclass);                         // unregister the device class
    class_destroy(myclass);                            // remove the device class
    unregister_chrdev(my_majornumber, DEVICE_NAME);    // unregister the major number
    //i2c_del_adapter(&my_adapter);
}

/**
 * @brief This function is called whenever device I2C has been opened from the user space.
 * 
 * @param inodep 
 * @param filep 
 * @return int 
 */
static int my_open(struct inode *inodep, struct file *filep)
{

    struct i2c_client *client;
    struct i2c_adapter *temp_adapter;
    unsigned int minor_number;
    printk(KERN_INFO "Device has been opened.\n");

    minor_number = iminor(inodep);

    temp_adapter = i2c_get_adapter(minor_number);
    if (!temp_adapter)
    {
        return -ENODEV;
    }

    client = kzalloc(sizeof(*client), GFP_KERNEL);
    if (!client)
    {
        i2c_put_adapter(temp_adapter);
        return -ENOMEM;
    }
    snprintf(client->name, I2C_NAME_SIZE, "myi2c %d", temp_adapter->nr);

    client->adapter = temp_adapter;
    filep->private_data = client;

    return 0;
}

/**
 * @brief This function is called when the I2C is being written from the user space.
 * 
 * @param filep 
 * @param buffer 
 * @param len 
 * @param offset 
 * @return ssize_t 
 */
static ssize_t my_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset)
{

    int res;
    char *tmp;
    struct i2c_client *client;
    client = filep->private_data;
    printk(KERN_INFO "Writing data using I2C.\n");

    if (len > 8192)
        len = 8192;

    tmp = memdup_user(buffer, len);
    if (IS_ERR(tmp))
        return PTR_ERR(tmp);

    pr_debug("myi2c: i2c-%d writing %zu bytes.\n",
             iminor(file_inode(filep)), len);

    res = i2c_master_send(client, tmp, len);
    kfree(tmp);
    return res;

}

/**
 * @brief This function is called when data is being read from I2C in user space.
 * 
 * @param filep 
 * @param buffer 
 * @param len 
 * @param offset 
 * @return ssize_t 
 */
static ssize_t my_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)
{
        char *tmp;
    int res;
    struct i2c_client *client;

    printk(KERN_INFO "Reading data using I2C.\n");
    client = filep->private_data;

    if (len > 8192)
        len = 8192;

    tmp = kmalloc(len, GFP_KERNEL);
    if (tmp == NULL)
        return -ENOMEM;

    pr_debug("myi2c: i2c-%d reading %zu bytes.\n",
             iminor(file_inode(filep)), len);

    res = i2c_master_recv(client, tmp, len);
    if (res >= 0)
        res = copy_to_user(buffer, tmp, len) ? -EFAULT : res;
    kfree(tmp);
    return res;

}

/**
 * @brief This function is called whenever ioctl system call is called from user space.
 * 
 * @param filep 
 * @param cmd 
 * @param arg 
 * @return int 
 */
static long my_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    struct i2c_client *client;
    printk(KERN_INFO "Entered IOCTL.\n");
    client = filep->private_data;

    dev_dbg(&client->adapter->dev, "ioctl, cmd=0x%02x, arg=0x%02lx\n",
            cmd, arg);

    switch (cmd)
    {
    case I2C_SLAVE:
        break;
    }
    return 0;
}
/**
 * @brief This function is called whenever the device I2C has been closed from user space.
 * 
 * @param inodep 
 * @param filep 
 * @return int 
 */
static int my_close(struct inode *inodep, struct file *filep)
{
    struct i2c_client *client;
    printk(KERN_INFO "Device closed\n");

    client = filep->private_data;

    i2c_put_adapter(client->adapter);
    kfree(client);
    filep->private_data = NULL;

    return 0;
}

module_init(myi2c_init);
module_exit(myi2c_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siddhant Jajoo and Satya Mehta");
MODULE_DESCRIPTION("I2C Kernel Driver");
MODULE_VERSION("0.1");
