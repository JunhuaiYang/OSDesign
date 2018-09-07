#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>

/*处理版本问题CONFIG_MODVERSIONS */ 
#if CONFIG_MODVERSIONS==1 
#define MODVERSIONS 
#include "linux/version.h" 
#endif 

#define DEVICE_NUM 0                   //随机产生一个设备号


// 打开关闭 读写入口
static int mydev_open(struct inode *inode, struct file *filp);
static int mydev_release(struct inode *inode, struct file *filp);
static ssize_t mydev_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t mydev_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos);

//file_operations结构是字符驱动程序的相关结构
static struct file_operations mydev_fops = {
    .read = mydev_read,
    .write = mydev_write,
    .open = mydev_open,
    .release = mydev_release,
};

// 必须要申明为静态变量和静态函数
static int device_num = 0;             //用来保存创建成功后的设备号
static char buffer[1024] = "Hello World! This is YJH's device!\n"; //数据缓冲区
static int open_process = 0;                //打开设备的进程数，用于内核的互斥

//打开设备
static int mydev_open(struct inode *inode, struct file *filp)
{
  // MAJOR和MINOR返回主次设备号
  printk("mydev: Main device number is %d  \n Slave device number is %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev));
  printk("mydev: Now device has opend! \n");
  // 未被占用
  if (open_process == 0)
  {
    open_process++; //标志占用
    try_module_get(THIS_MODULE);
    return 0;
  }
  // 被占用
  else
  {
    printk(KERN_ALERT "Another process open the char device.\n"); //进程挂起
    return -1;
  }
}

//释放设备
static int mydev_release(struct inode *inode, struct file *filp)
{
  open_process--; //占用解除
  printk("mydev: The device has released!\n");
  module_put(THIS_MODULE);
  return 0;
}

//读
static ssize_t mydev_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
  if (copy_to_user(buf, buffer, sizeof(buffer))) //从内核到用户 
  {
    return -1;
  }
  return sizeof(buffer);
}

//写
static ssize_t mydev_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos)
{
  if (copy_from_user(buffer, buf, sizeof(buffer))) //用户到内核
  {
    return -1;
  }
  return sizeof(buffer);
}


//注册设备函数
static int __init mydev_init(void)
{
  int result;
  printk(KERN_ALERT "Begin to init Char Device!"); //注册设备

  //向系统的字符登记表登记一个字符设备
  result = register_chrdev(DEVICE_NUM, "mydev_yjh", &mydev_fops);
  if (result < 0)
  {
    printk(KERN_WARNING "mydev: register failure\n");
    return -1;
  }
  else
  {
    // 注册成功
    printk("mydev: register success!\n");
    device_num = result;
    return 0;
  }
}

//注销设备函数
static void __exit mydev_exit(void)
{
  printk(KERN_ALERT "Unloading...\n");
  unregister_chrdev(device_num, "mydev"); //字符设备卸载
  printk("mydev: unregister success!\n");
}

//模块宏定义
module_init(mydev_init);
module_exit(mydev_exit);
// 模块许可协议
MODULE_LICENSE("GPL");
