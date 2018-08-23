#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#if CONFIG_MODVERSIONS == 1
#define MODVERSIONS
#include <linux/version.h>
#endif
#define DEVICE_NUM 0                   //随机产生一个设备号
static int device_num = 0;             //用来保存创建成功后的设备号
static char buffer[1024] = "mydriver"; //数据缓冲区
static int open_nr = 0;                //打开设备的进程数，用于内核的互斥

//函数声明
static int mydriver_open(struct inode *inode, struct file *filp);
static int mydriver_release(struct inode *inode, struct file *filp);
static ssize_t mydriver_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t mydriver_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos);
//填充file_operations结构相关入口
static struct file_operations mydriver_fops = {
    .read = mydriver_read,
    .write = mydriver_write,
    .open = mydriver_open,
    .release = mydriver_release,
};

//打开函数
static int mydriver_open(struct inode *inode, struct file *filp)
{
  printk("\nMain device is %d, and the slave device is %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev));
  if (open_nr == 0)
  {
    open_nr++;
    try_module_get(THIS_MODULE);
    return 0;
  }
  else
  {
    printk(KERN_ALERT "Another process open the char device.\n"); //进程挂起
    return -1;
  }
}
//读函数
static ssize_t mydriver_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
  //if (buf == NULL) return 0;
  if (copy_to_user(buf, buffer, sizeof(buffer))) //读缓冲
  {
    return -1;
  }
  return sizeof(buffer);
}
//写函数,将用户的输入字符串写入
static ssize_t mydriver_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos)
{
  //if (buf == NULL) return 0;
  if (copy_from_user(buffer, buf, sizeof(buffer))) //写缓冲
  {
    return -1;
  }
  return sizeof(buffer);
}
//释放设备函数
static int mydriver_release(struct inode *inode, struct file *filp)
{
  open_nr--; //进程数减1
  printk("The device is released!\n");
  module_put(THIS_MODULE);
  return 0;
}
//注册设备函数
static int __init mydriver_init(void)
{
  int result;
  printk(KERN_ALERT "Begin to init Char Device!"); //注册设备
  //向系统的字符登记表登记一个字符设备
  result = register_chrdev(DEVICE_NUM, "mydriver", &mydriver_fops);
  if (result < 0)
  {
    printk(KERN_WARNING "mydriver: register failure\n");
    return -1;
  }
  else
  {
    printk("mydriver: register success!\n");
    device_num = result;
    return 0;
  }
}
//注销设备函数
static void __exit mydriver_exit(void)
{
  printk(KERN_ALERT "Unloading...\n");
  unregister_chrdev(device_num, "mydriver"); //注销设备
  printk("unregister success!\n");
}
//模块宏定义
module_init(mydriver_init);
module_exit(mydriver_exit);
MODULE_LICENSE("GPL");
