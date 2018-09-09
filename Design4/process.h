/***********************************************************************
 * @file process.h
     PROCESS
 * @brief   header file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/
#ifndef __PROCESS_h__
#define __PROCESS_h__

#include "MainWindow.h"

#define P_COLUMN 5

enum
{
    NAME_COLUMN,  //进程名列
    PID_COLUMN,    //进程PID列
    STATUS_COLUMN, //进程状态列
    CPU_COLUMN,  //CPU信息列
    MEMORY_COLUMN,//内存信息列
    NP_COLUMNS
};

void CreateProcess(GtkWidget* notebook);
void GetInfo(char *string, char **info);
void GetProcessInfo(GtkListStore *store);

#endif // __PROCESS_h__