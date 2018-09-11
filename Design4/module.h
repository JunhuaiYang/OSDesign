/***********************************************************************
 * @file module.h
     MODULE
 * @brief   header file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/
#ifndef __MODULE_h__
#define __MODULE_h__

#include "MainWindow.h"
#include "process.h"

#define M_COLUMN 4

enum
{
    M_NAME_COLUMN,  //模块名列
    M_MEMORY_COLUMN,//内存信息列
    M_COUNT_COLUMN,  //使用计数列
    M_STATUS_COLUMN, //进程状态列
    M_COLUMNS
};

void CreateModule(GtkWidget* notebook);
void GetModulesInfo(GtkListStore *store);
gboolean PopModuleMenu(GtkWidget *widget, GdkEventButton *event, gpointer data);
gboolean ModuleRefresh(gpointer data);
gboolean ModuleNew(gpointer data);
gboolean ModuleDestory(gpointer data);
void NewModule(GtkWidget *w,GtkFileSelection *fs);
void DestroyFs(GtkWidget *w,GtkFileSelection *fs);

#endif // __MODULE_h__