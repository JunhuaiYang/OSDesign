/***********************************************************************
 * @file memory.c
     MEMORY
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "memory.h"

void CreateMemory(GtkWidget* notebook)
{
    GtkWidget *table;

    // 需要表格布局 创建10行10列的表格
    table = gtk_table_new(10, 10, TRUE);
    /* 将该标签页加入到notebook中 */
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table, gtk_label_new("内存"));

    // 创建frame来装各种控件
    GtkWidget *CPU_frame1 = gtk_frame_new("内存详细情况");
    GtkWidget *CPU_frame2 = gtk_frame_new("内存使用曲线");
    GtkWidget *CPU_frame3 = gtk_frame_new("交换分区使用曲线");

    gtk_table_attach_defaults(GTK_TABLE(table), CPU_frame1, 0, 3, 0, 10);
    gtk_table_attach_defaults(GTK_TABLE(table), CPU_frame2, 3, 10, 0, 6);
    gtk_table_attach_defaults(GTK_TABLE(table), CPU_frame3, 3, 10, 6, 10);
}