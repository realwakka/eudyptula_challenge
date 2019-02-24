#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/uaccess.h>

static DEFINE_SEMAPHORE(foo_sem);
static struct dentry *eudyptula_dentry;

#define MY_ID "DEBUGID"
#define MY_ID_LEN 8

#define FOO_VALUE_LEN 255
static char foo_value[FOO_VALUE_LEN];

static ssize_t id_fop_read(struct file *fp, char __user *user_buffer, 
                           size_t count, loff_t *position) {
  if (*position != 0)
    return 0;
  if ((count < MY_ID_LEN) || (copy_to_user(user_buffer, MY_ID, MY_ID_LEN)))
    return -EINVAL;

  *position += count;
  return count;
}

static ssize_t id_fop_write(struct file *fp, const char __user *user_buffer, 
                            size_t count, loff_t *position) {

  char buf[MY_ID_LEN];
  
  if ((count != MY_ID_LEN)
      || (copy_from_user(buf, MY_ID, MY_ID_LEN))
      || (strncmp(buf, MY_ID, MY_ID_LEN) != 0))
    return -EINVAL;
  else
    return count;
}

static ssize_t jiffies_fop_read(struct file *fp, char __user *user_buffer, 
                                size_t count, loff_t *position) {

  if (*position != 0)
    return 0;


  if (count < sizeof(jiffies)
      || copy_to_user(user_buffer, (void*)&jiffies, sizeof(jiffies))) {
    return -EINVAL;
  }


  *position += count;

  return sizeof(jiffies);
}

static ssize_t foo_fop_read(struct file *fp, char __user *user_buffer, 
                            size_t count, loff_t *position) {

  if (*position != 0)
    return 0;

  if (count > FOO_VALUE_LEN) {
    return -EINVAL;
  }
  
  down(&foo_sem);
  if (copy_to_user(user_buffer, foo_value, count)) {
    return -EINVAL;
  }
  up(&foo_sem);

  return count;
}

static ssize_t foo_fop_write(struct file *fp, const char __user *user_buffer, 
                             size_t count, loff_t *position) {

  if (*position != 0)
    return 0;

  if (count > FOO_VALUE_LEN) {
    return -EINVAL;
  }
  
  down(&foo_sem);
  if (copy_from_user(foo_value, user_buffer, count)) {
    return -EINVAL;
  }
  up(&foo_sem);
  
  return count;
}

const static struct file_operations id_fops = {
  .read = id_fop_read,
  .write = id_fop_write
};

const static struct file_operations jiffies_fops = {
  .read = jiffies_fop_read
};

const static struct file_operations foo_fops = {
  .read = foo_fop_read,
  .write = foo_fop_write
};


static int hello_init(void)
{
  printk(KERN_DEBUG "Hello World!\n");
  eudyptula_dentry = debugfs_create_dir("eudyptula", NULL);

  debugfs_create_file("id", 0644, eudyptula_dentry, NULL, &id_fops);
  debugfs_create_file("jiffies", 0644, eudyptula_dentry, NULL, &jiffies_fops);
  debugfs_create_file("foo", 0644, eudyptula_dentry, NULL, &foo_fops);
  return 0;
}

static void hello_exit(void)
{
  debugfs_remove_recursive(eudyptula_dentry);
  printk(KERN_DEBUG "See you later.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("7c1caf2f50d1");
MODULE_DESCRIPTION("Just a module");
