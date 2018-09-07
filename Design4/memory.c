/***********************************************************************
 * @file memory.c
     MEMORY
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "memory.h"

MEMO_INFO memo_info;

// 刷新标签
GtkWidget *label_memo_1;
GtkWidget *label_memo_2;
GtkWidget *label_memo_3;
GtkWidget *label_memo_4;
GtkWidget *label_memo_5;
GtkWidget *label_memo_6;
GtkWidget *label_memo_7;
GtkWidget *label_memo_8;
GtkWidget *label_memo_9;

void CreateMemory(GtkWidget* notebook)
{
    GtkWidget *table;

    GtkWidget *vbox1;
    GtkWidget *vbox3;

    // 需要表格布局 创建10行10列的表格
    table = gtk_table_new(10, 10, TRUE);
    /* 将该标签页加入到notebook中 */
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table, gtk_label_new("内存"));

    // 创建frame来装各种控件
    GtkWidget *MEMO_frame1 = gtk_frame_new("内存详细情况");
    GtkWidget *MEMO_frame2 = gtk_frame_new("内存使用曲线");
    GtkWidget *MEMO_frame3 = gtk_frame_new("交换分区详细情况");
    GtkWidget *MEMO_frame4 = gtk_frame_new("交换分区使用曲线");

    gtk_table_attach_defaults(GTK_TABLE(table), MEMO_frame1, 0, 3, 0, 6);
    gtk_table_attach_defaults(GTK_TABLE(table), MEMO_frame3, 0, 3, 6, 10);
    gtk_table_attach_defaults(GTK_TABLE(table), MEMO_frame2, 3, 10, 0, 6);
    gtk_table_attach_defaults(GTK_TABLE(table), MEMO_frame4, 3, 10, 6, 10);

    // 创建装文本的vbox
    vbox1 = gtk_vbox_new(TRUE, 0);
    gtk_container_border_width(GTK_CONTAINER(vbox1), 2);
    gtk_container_add(GTK_CONTAINER(MEMO_frame1), vbox1);

    vbox3 = gtk_vbox_new(TRUE, 0);
    gtk_container_border_width(GTK_CONTAINER(vbox3), 2);
    gtk_container_add(GTK_CONTAINER(MEMO_frame3), vbox3);

    ShowMemoInfo(vbox1);
    ShowSwapInfo(vbox3);

    // 内存使用率
    g_timeout_add(1000, UpdateMemo, NULL);
}

void ShowMemoInfo(GtkWidget *vbox)
{
    GtkWidget *table;
    // 需要表格布局 创建7行1列的表格
    table = gtk_table_new(5, 1, TRUE);
    gtk_container_add(GTK_CONTAINER(vbox), table);

    GtkWidget *vbox1 = gtk_vbox_new(TRUE, 0);
    GtkWidget *vbox2 = gtk_vbox_new(TRUE, 0);
    GtkWidget *vbox3 = gtk_vbox_new(TRUE, 0);
    GtkWidget *vbox4 = gtk_vbox_new(TRUE, 0);
    GtkWidget *vbox5 = gtk_vbox_new(TRUE, 0);


    gtk_table_attach_defaults(GTK_TABLE(table), vbox1, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), vbox2, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), vbox3, 0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), vbox4, 0, 1, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), vbox5, 0, 1, 4, 5);


    // 每个表格的布局
    GtkWidget *label;

    label = gtk_label_new("内存使用率");
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
    set_widget_font_size(label, 14, FALSE);
    gtk_container_add(GTK_CONTAINER(vbox1), label);

    label = gtk_label_new("(已使用) 总内存");
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
    set_widget_font_size(label, 14, FALSE);
    gtk_container_add(GTK_CONTAINER(vbox2), label);

    label = gtk_label_new("可用内存");
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
    set_widget_font_size(label, 14, FALSE);
    gtk_container_add(GTK_CONTAINER(vbox3), label);

    label = gtk_label_new("已缓冲 / 已缓存");
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
    set_widget_font_size(label, 14, FALSE);
    gtk_container_add(GTK_CONTAINER(vbox4), label);

    label = gtk_label_new("活跃内存 / 不活跃内存");
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
    set_widget_font_size(label, 14, FALSE);
    gtk_container_add(GTK_CONTAINER(vbox5), label);

    label_memo_1 = gtk_label_new("123");
    gtk_misc_set_alignment(GTK_MISC(label_memo_1), 0, 0.5);
    gtk_container_add(GTK_CONTAINER(vbox1), label_memo_1);

    label_memo_2 = gtk_label_new("34");
    gtk_misc_set_alignment(GTK_MISC(label_memo_2), 0, 0.5);
    gtk_container_add(GTK_CONTAINER(vbox2), label_memo_2);

    label_memo_3 = gtk_label_new("345");
    gtk_misc_set_alignment(GTK_MISC(label_memo_3), 0, 0.5);
    gtk_container_add(GTK_CONTAINER(vbox3), label_memo_3);

    label_memo_4 = gtk_label_new("123");
    gtk_misc_set_alignment(GTK_MISC(label_memo_4), 0, 0.5);
    gtk_container_add(GTK_CONTAINER(vbox4), label_memo_4);

    label_memo_5 = gtk_label_new("324");
    gtk_misc_set_alignment(GTK_MISC(label_memo_5), 0, 0.5);
    gtk_container_add(GTK_CONTAINER(vbox5), label_memo_5);

}

void ShowSwapInfo(GtkWidget *vbox)
{
    GtkWidget *table;
    // 需要表格布局 创建7行1列的表格
    table = gtk_table_new(4, 1, TRUE);
    gtk_container_add(GTK_CONTAINER(vbox), table);

    GtkWidget *vbox1 = gtk_vbox_new(TRUE, 0);
    GtkWidget *vbox2 = gtk_vbox_new(TRUE, 0);
    GtkWidget *vbox3 = gtk_vbox_new(TRUE, 0);
    GtkWidget *vbox4 = gtk_vbox_new(TRUE, 0);


    gtk_table_attach_defaults(GTK_TABLE(table), vbox1, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), vbox2, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), vbox3, 0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), vbox4, 0, 1, 3, 4);


    // 每个表格的布局
    GtkWidget *label;

    label = gtk_label_new("交换分区使用率");
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
    set_widget_font_size(label, 12, FALSE);
    gtk_container_add(GTK_CONTAINER(vbox1), label);

    label = gtk_label_new("(已使用) 总交换分区");
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
    set_widget_font_size(label, 12, FALSE);
    gtk_container_add(GTK_CONTAINER(vbox2), label);

    label = gtk_label_new("可用交换分区");
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
    set_widget_font_size(label, 12, FALSE);
    gtk_container_add(GTK_CONTAINER(vbox3), label);

    label = gtk_label_new("等待写回 / 正在写回");
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
    set_widget_font_size(label, 12, FALSE);
    gtk_container_add(GTK_CONTAINER(vbox4), label);

    label_memo_6 = gtk_label_new("123");
    gtk_misc_set_alignment(GTK_MISC(label_memo_6), 0, 0.5);
    gtk_container_add(GTK_CONTAINER(vbox1), label_memo_6);

    label_memo_7 = gtk_label_new("34");
    gtk_misc_set_alignment(GTK_MISC(label_memo_7), 0, 0.5);
    gtk_container_add(GTK_CONTAINER(vbox2), label_memo_7);

    label_memo_8 = gtk_label_new("345");
    gtk_misc_set_alignment(GTK_MISC(label_memo_8), 0, 0.5);
    gtk_container_add(GTK_CONTAINER(vbox3), label_memo_8);

    label_memo_9 = gtk_label_new("123");
    gtk_misc_set_alignment(GTK_MISC(label_memo_9), 0, 0.5);
    gtk_container_add(GTK_CONTAINER(vbox4), label_memo_9);

}

long int GetOneMemoInfo(char *name)
{
    int fd;
    char buf[3000];
    char *p = NULL;
    char cache[30];
    long int data;

    if ((fd = open("/proc/meminfo", O_RDONLY)) == -1)
    {
        perror("fail to meminfo");
        return 0;
    }

    read(fd, buf, sizeof(buf));
    close(fd);
    // 查找子串
    p = strstr(buf, name);

    sscanf(p, "%s %ld",cache, &data);

    return data;
}

void GetMemoInfo(void)
{
    memo_info.MemTotal = GetOneMemoInfo("MemTotal") ;
    memo_info.MemFree = GetOneMemoInfo("MemAvailable") ; //MemAvailable 才是系统可用内存 Free是剩余内存
    memo_info.Buffers = GetOneMemoInfo("Buffers") ;
    memo_info.Cached = GetOneMemoInfo("Cached") ;
    memo_info.Active = GetOneMemoInfo("Active") ;
    memo_info.Inactive = GetOneMemoInfo("Inactive") ;

    memo_info.SwapTotal = GetOneMemoInfo("SwapTotal") ;
    memo_info.SwapFree = GetOneMemoInfo("SwapFree") ;
    memo_info.Dirty = GetOneMemoInfo("Dirty") ;    
    memo_info.Writeback = GetOneMemoInfo("Writeback") ; 

    memo_info.memoredio = (float)(memo_info.MemTotal - memo_info.MemFree) / memo_info.MemTotal *100;
    memo_info.swapredio = (float)(memo_info.SwapTotal - memo_info.SwapFree) / memo_info.SwapTotal *100;

}

gint UpdateMemo(gpointer data)
{
    char string[128];
    // 更新信息
    GetMemoInfo();

    sprintf(string, "%.2f %%", memo_info.memoredio);
    gtk_label_set_text(GTK_LABEL(label_memo_1), string);

    sprintf(string, "(%.2fG) %.2fG", (memo_info.MemTotal - memo_info.MemFree) / 1024.0 /1024.0 ,
                    memo_info.MemTotal / 1024.0 /1024.0);
    gtk_label_set_text(GTK_LABEL(label_memo_2), string);

    sprintf(string, "%.2fM", memo_info.MemFree / 1024.0);
    gtk_label_set_text(GTK_LABEL(label_memo_3), string);

    sprintf(string, "%.2fM / %.2fM ", memo_info.Buffers / 1024.0, memo_info.Cached /1024.0);
    gtk_label_set_text(GTK_LABEL(label_memo_4), string);

    sprintf(string, "%.2fM / %.2fM ", memo_info.Active / 1024.0, memo_info.Inactive /1024.0);
    gtk_label_set_text(GTK_LABEL(label_memo_5), string);

    sprintf(string, "%.2f %%", memo_info.swapredio);
    gtk_label_set_text(GTK_LABEL(label_memo_6), string);

    sprintf(string, "(%.2fM) %.2fG", (memo_info.SwapTotal - memo_info.SwapFree) / 1024.0 ,
                     memo_info.SwapTotal / 1024.0 / 1024.0);
    gtk_label_set_text(GTK_LABEL(label_memo_7), string);

    sprintf(string, "%.2fM", memo_info.SwapFree /1024.0);
    gtk_label_set_text(GTK_LABEL(label_memo_8), string);

    sprintf(string, "%.2fM / %.2fM",memo_info.Dirty / 1024.0, memo_info.Writeback /1024.0);
    gtk_label_set_text(GTK_LABEL(label_memo_9), string);

    return 1;
}