#include "linux/kernerl.h" 
#include "linux/module.h" 
/*处理版本问题CONFIG_MODVERSIONS */ 

#if CONFIG_MODVERSIONS==1 
#define MODVERSIONS 
#include "linux/version.h" 
#endif 

int init_module() /* 模块初始化*/ 
{ 
  printk("hello world !\n");
  printk("I have runing in a kernel mod! \n"); 
  return 0; 
}
void cleanup_module() /* 模块卸载 */ 
{  
  printk(" I will shut down myself in kernel mode!\n)"; 
}  