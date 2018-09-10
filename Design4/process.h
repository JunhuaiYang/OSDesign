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
#include "cpu.h"

#define P_COLUMN 6
#define MAX_P 300

enum
{
    PID_COLUMN,    //进程PID列
    NAME_COLUMN,  //进程名列
    STATUS_COLUMN, //进程状态列
    CPU_COLUMN,  //CPU信息列
    MEMORY_COLUMN,//内存信息列
    PRIORITY_COLUMN,//内存信息列
    NP_COLUMNS
};

void CreateProcess(GtkWidget* notebook);
void GetProcessInfo(GtkListStore *store);
gboolean UpdateProcess (gpointer data);
gboolean PopMenu(GtkWidget *widget, GdkEventButton *event, gpointer data); 
gboolean KillProcess(gpointer data);
void ShowErrorDialog(char *info);
void ShowInfoDialog(char *info);
gboolean StopProcess(gpointer data);
gboolean ContinueProcess(gpointer data);

#endif // __PROCESS_h__