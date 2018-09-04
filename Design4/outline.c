/***********************************************************************
 * @file outline.c
     OUTLINE
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "outline.h"

void CreateOutline(GtkWidget *notebook)
{

    GtkWidget *hbox;
    GtkWidget *vbox;
    OUTLINE_INFO oinfo;

    // 创建水平盒装容器并添加
    hbox = gtk_hbox_new(TRUE, 5);
    gtk_container_border_width(GTK_CONTAINER(hbox), 5);

    /* 将该标签页加入到notebook中 */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, gtk_label_new("系统概要"));

    //用 pixbuf 打开图片
    GdkPixbuf *src = gdk_pixbuf_new_from_file("./image/ubuntu.svg", NULL);
    //修改图片大小
    GdkPixbuf *dst = gdk_pixbuf_scale_simple(src, 150, 150, GDK_INTERP_BILINEAR);
    GtkWidget *image = gtk_image_new_from_pixbuf(dst);
    //将image_two添加到hbox中
    gtk_container_add(GTK_CONTAINER(hbox), image);
    //pixbuf使用完需要释放其中资源
    g_object_unref(src);
    g_object_unref(dst);

    // 创建垂直盒装容器并添加
    vbox = gtk_vbox_new(TRUE, 5);
    gtk_container_border_width(GTK_CONTAINER(vbox), 5);
    gtk_container_add(GTK_CONTAINER(hbox), vbox);

    GtkWidget *label_one = gtk_label_new("label one\nlabel");
    //将label_one添加到vbox中
    gtk_container_add(GTK_CONTAINER(vbox), label_one);
    label_one = gtk_label_new("label two\nlabel");
    // 对齐 第二个参数 0为左  第三个参数 0为上
    gtk_misc_set_alignment(GTK_MISC(label_one), 0, 0.5);
    //将label_one添加到vbox中
    gtk_container_add(GTK_CONTAINER(vbox), label_one);

    GetOutlineInfo(&oinfo);
}

void GetOutlineInfo(p_outline_info oinfo)
{
    int fd;

    // 读取主机名
    //打开存储系统平均负载的proc文件
    if ((fd = open("/proc/sys/kernel/hostname", O_RDONLY)) == -1)
    {
        perror("fail to hostname");
        strcpy("none", oinfo->hostname);
    }
    else
    {

    }
    close(fd);

    if ((fd = open("/etc/issue", O_RDONLY)) == -1)
    {
        perror("fail to os");
        strcpy("none", oinfo->os);
    }
    else
    {

    }
    close(fd);

    if ((fd = open("/proc/version", O_RDONLY)) == -1)
    {
        perror("fail to kernel");
        strcpy("none", oinfo->kernel);
    }
    else
    {

    }
    close(fd);

}