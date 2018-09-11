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

#endif // __MODULE_h__