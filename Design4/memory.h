/***********************************************************************
 * @file memory.h
     MEMORY
 * @brief   header file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/
#ifndef __MEMORY_h__
#define __MEMORY_h__

#include "MainWindow.h"

typedef struct memoinfo
{
    long int MemTotal;
    long int MemFree;
    long int Buffers; //缓冲
    long int Cached;  //缓存
    long int Active;
    long int Inactive;

    long int SwapTotal;
    long int SwapFree;
    long int Dirty;     //等待写回的数据大小
    long int Writeback;  //正在写回的数据大小

    float memoredio;
    float swapredio;
}MEMO_INFO, *p_memoinfo;

void CreateMemory(GtkWidget* notebook);
long int GetOneMemoInfo(char *name);
void ShowMemoInfo(GtkWidget *vbox);
void ShowSwapInfo(GtkWidget *vbox);
void GetMemoInfo(void);
gint UpdateMemo(gpointer data);


#endif // __MEMORY_h__