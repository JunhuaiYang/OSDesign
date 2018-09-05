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

extern CPU_INFO cpu_info;

void CreateCPU(GtkWidget* notebook);
void GetOneInfo(char *path, char *name, char* info);
void ShowLabel(GtkWidget *vbox, const char *text);
void ShowCPUInfo(GtkWidget *vbox);
unsigned int strstrcount(char *str1, char *str2);
void GetProcessor(char *str);
void GetCPUInfo(void);
gint ShowRatio(gpointer data);







#endif // __CPU_h__