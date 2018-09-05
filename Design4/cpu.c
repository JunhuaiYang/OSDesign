/***********************************************************************
 * @file cpu.c
     CPU
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "cpu.h"

// 申明为全局变量
CPU_INFO cpu_info;

void CreateCPU(GtkWidget *notebook)
{
    GtkWidget *vbox1;
    GtkWidget *vbox3;
    GtkWidget *table;

    // 需要表格布局 创建10行10列的表格
    table = gtk_table_new(10, 10, TRUE);

    /* 将该表格加入到notebook中 */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table, gtk_label_new("CPU"));

    // 创建frame来装各种控件
    GtkWidget *CPU_frame1 = gtk_frame_new("CPU使用情况");
    GtkWidget *CPU_frame2 = gtk_frame_new("CPU使用历史曲线");
    GtkWidget *CPU_frame3 = gtk_frame_new("CPU基本信息");

    gtk_table_attach_defaults(GTK_TABLE(table), CPU_frame1, 0, 3, 0, 6);
    gtk_table_attach_defaults(GTK_TABLE(table), CPU_frame2, 3, 10, 0, 6);
    gtk_table_attach_defaults(GTK_TABLE(table), CPU_frame3, 0, 10, 6, 10);

    // 创建装文本的vbox
    vbox1 = gtk_vbox_new(TRUE, 5);
    gtk_container_border_width(GTK_CONTAINER(vbox1), 5);
    gtk_container_add(GTK_CONTAINER(CPU_frame1), vbox1);

    vbox3 = gtk_vbox_new(TRUE, 5);
    gtk_container_border_width(GTK_CONTAINER(vbox3), 5);
    gtk_container_add(GTK_CONTAINER(CPU_frame3), vbox3);

    // 获取CPU基本信息并显示
    GetCPUInfo();
    ShowCPUInfo(vbox3);
}

void GetOneInfo(char *path, char *name, char *info)
{
    int fd = open(path, O_RDONLY);
    char buf[3000];
    char *p = NULL;
    char *t = info;

    read(fd, buf, sizeof(buf));
    close(fd);
    // 查找子串
    p = strstr(buf, name);

    /* 跳过标题 */
    for (; *p != ':'; p++)
        ;
    // 跳过空字符
    p += 2;
    /* 读取数值 */
    while (*p != '\n')
    {
        *(t++) = *(p++);
    }
    *t = '\0';
}

// 查找子字符串出现次数  获得处理器线程数
// str1 为被查找的串
unsigned int strstrcount(char *str1, char *str2)
{
    char *str = str1;
    unsigned int c = 0;
    while ((str = strstr(str, str2)) != NULL)
    {
        c++;
        str++;
    }
    return c;
}

void GetProcessor(char *str)
{
    char buf[5000];
    int fd = open("/proc/cpuinfo", O_RDONLY);
    read(fd, buf, sizeof(buf));

    unsigned int i = strstrcount(buf,"processor");

    sprintf(str,"%hd",i);
}

void GetCPUInfo(void)
{
    // model_name
    GetOneInfo("/proc/cpuinfo", "model name", cpu_info.model_name);

    // cache_size
    GetOneInfo("/proc/cpuinfo", "cache size", cpu_info.cache_size);

    // clock_speed
    GetOneInfo("/proc/cpuinfo", "cpu MHz", cpu_info.clock_speed);

    // cores
    GetOneInfo("/proc/cpuinfo", "cpu cores", cpu_info.cores);

    // address_sizes
    GetOneInfo("/proc/cpuinfo", "address sizes", cpu_info.address_sizes);

    // processor
    GetProcessor(cpu_info.processor);

}

void ShowCPUInfo(GtkWidget *vbox)
{
    char text[50];

    sprintf(text, "\t\t型号\t\t： %s", cpu_info.model_name);
    ShowLabel(vbox, text);

    sprintf(text, "\t\t核心\t\t：%s", cpu_info.cores);
    ShowLabel(vbox, text);

    sprintf(text, "\t\t逻辑处理器\t：%s", cpu_info.processor);
    ShowLabel(vbox, text);

    sprintf(text, "\t\t主频\t\t：%s MHz", cpu_info.clock_speed);
    ShowLabel(vbox, text);

    sprintf(text, "\t\t缓存\t\t：%s", cpu_info.cache_size);
    ShowLabel(vbox, text);

    sprintf(text, "\t\t寻址位数\t：%s", cpu_info.address_sizes);
    ShowLabel(vbox, text);

}

void ShowLabel(GtkWidget *vbox, const char *text)
{
    GtkWidget *label;

    label = gtk_label_new("");
    // 对齐 第二个参数 0为左  第三个参数 0为上
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
    gtk_label_set_text(GTK_LABEL(label), text);
    gtk_container_add(GTK_CONTAINER(vbox), label);
}