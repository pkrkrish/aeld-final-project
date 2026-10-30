#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Self");
MODULE_DESCRIPTION("AELD Final Project - Real-Time Telemetry Character Device Driver");
MODULE_VERSION("0.1");

#define DEVICE_NAME "rpi_telemetry"
#define BUF_LEN 80

static int major_num;
static char msg_buffer[BUF_LEN];
static int msg_len = 0;

static int device_open(struct inode *inode, struct file *file) {
    pr_info("rpi_telemetry: Device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    pr_info("rpi_telemetry: Device closed\n");
    return 0;
}

static ssize_t device_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    int bytes_read = 0;
    if (*offset >= msg_len) return 0;
    
    while (len && (*offset < msg_len)) {
        put_user(msg_buffer[*offset], buffer++);
        len--;
        (*offset)++;
        bytes_read++;
    }
    return bytes_read;
}

static ssize_t device_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    pr_info("rpi_telemetry: Write operation not supported in stub\n");
    return -EINVAL;
}

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

static int __init rpi_chardev_init(void) {
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) {
        pr_alert("rpi_telemetry: Failed to register major number\n");
        return major_num;
    }
    pr_info("rpi_telemetry: Registered with major number %d\n", major_num);
    return 0;
}

static void __exit rpi_chardev_exit(void) {
    unregister_chrdev(major_num, DEVICE_NAME);
    pr_info("rpi_telemetry: Unregistered device\n");
}

module_init(rpi_chardev_init);
module_exit(rpi_chardev_exit);