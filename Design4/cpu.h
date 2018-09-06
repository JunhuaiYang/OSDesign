/***********************************************************************
 * @file cpu.h
     CPU
 * @brief   header file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/
#ifndef __CPU_h__
#define __CPU_h__

#include "MainWindow.h"


typedef struct cpuInfo
{
    char model_name[50];
    char cache_size[20];
    char clock_speed[20];
    char cores[10];
    char processor[50];  //逻辑处理器数
    char address_sizes[50];  //寻址位数
} CPU_INFO, *p_cpuinfo;

typedef struct statinfo
{
    long int user;  // 用户模式
    long int nice;  // 低优先级用户模式
    long int sys;  // 内核模式
    long int idle;  // 空闲的处理器时间
    long int iowait;  // io等待
    long int irq;  // 硬中断时间
    long int softirq;  // 软中断时间
    long int processes;  //运行进程
    long int procs_running;  //正在运行
    long int procs_blocked;  //阻塞
}STAT_INFO, *p_statinfo;

extern CPU_INFO cpu_info;

void CreateCPU(GtkWidget* notebook);
void GetOneInfo(char *path, char *name, char* info);
void ShowLabel(GtkWidget *vbox, const char *text);
void ShowCPUInfo(GtkWidget *vbox);
unsigned int strstrcount(char *str1, char *str2);
void GetProcessor(char *str);
void GetCPUInfo(void);
void ShowRatio(GtkWidget *data);
gint UpdateRatio(gpointer data);
void GetStat(p_statinfo istat);





#endif // __CPU_h__