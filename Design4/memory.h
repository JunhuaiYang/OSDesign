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

#define LENGTH_M 100

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

extern MEMO_INFO memo_info;
extern gint swap_graph[LENGTH_M];
extern gint mem_graph[LENGTH_M];

void CreateMemory(GtkWidget* notebook);
long int GetOneMemoInfo(char *name);
void ShowMemoInfo(GtkWidget *vbox);
void ShowSwapInfo(GtkWidget *vbox);
void GetMemoInfo(void);
gint UpdateMemo(gpointer data);
gboolean m_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
gboolean m_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);
gboolean s_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
gboolean s_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);


void DrawMemGraph(void);
void DrawSwapGraph(void);


#endif // __MEMORY_h__