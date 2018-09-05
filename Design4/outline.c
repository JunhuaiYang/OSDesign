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
    GdkPixbuf *src;

    // 清空结构图脏数据
    memset(&oinfo, '\0', sizeof(oinfo));

    // 创建水平盒装容器并添加
    hbox = gtk_hbox_new(TRUE, 5);
    gtk_container_border_width(GTK_CONTAINER(hbox), 5);

    /* 将该标签页加入到notebook中 */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, gtk_label_new("系统概要"));

    // 获得系统信息
    GetOutlineInfo(&oinfo);

    if (!strcmp("Ubuntu", oinfo.os))
    {
        //用 pixbuf 打开图片
        src = gdk_pixbuf_new_from_file("./image/ubuntu.svg", NULL);
    }
    else if (!strcmp("Arch", oinfo.os))
    {
        src = gdk_pixbuf_new_from_file("./image/arch.png", NULL);
    }
    else if (!strcmp("Debian", oinfo.os))
    {
        src = gdk_pixbuf_new_from_file("./image/debian.png", NULL);
    }
    else if (!strcmp("Fedora", oinfo.os))
    {
        src = gdk_pixbuf_new_from_file("./image/fedora.png", NULL);
    }
    else
        src = gdk_pixbuf_new_from_file("./image/unknow.jpg", NULL);

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
    gtk_container_border_width(GTK_CONTAINER(vbox), 20);
    gtk_container_add(GTK_CONTAINER(hbox), vbox);

    // 显示信息
    ShowInfo(vbox, oinfo);
}

void GetOutlineInfo(p_outline_info oinfo)
{
    int fd;
    int bytes;
    char buf[50];
    char *pchar, *tp;

    // 读取主机名
    //打开存储系统平均负载的proc文件
    if ((fd = open("/proc/sys/kernel/hostname", O_RDONLY)) == -1)
    {
        perror("fail to hostname");
        strcpy("none", oinfo->hostname);
    }
    else
    {
        bytes = read(fd, buf, sizeof(buf));
        buf[bytes] = '\0';
        strcpy(oinfo->hostname, buf);
    }
    close(fd);

    // 获得系统及版本号
    if ((fd = open("/etc/issue", O_RDONLY)) == -1)
    {
        perror("fail to os");
        strcpy("none", oinfo->os);
    }
    else
    {
        bytes = read(fd, buf, sizeof(buf));
        buf[bytes - 1] = '\0';
        strcpy(oinfo->version, buf);
        pchar = buf;
        tp = oinfo->os;
        while (*pchar != ' ')
            *(tp++) = *(pchar++);
        tp = '\0';
    }
    close(fd);

    if ((fd = open("/proc/version", O_RDONLY)) == -1)
    {
        perror("fail to kernel");
        strcpy("none", oinfo->kernel);
    }
    else
    {
        bytes = read(fd, buf, 50);
        buf[50] = '\0';
        pchar = buf + 14;
        tp = oinfo->kernel;
        while (*pchar != ' ')
            *(tp++) = *(pchar++);
        tp = '\0';
    }
    close(fd);
}

void ShowInfo(GtkWidget *vbox, OUTLINE_INFO oinfo)
{
    char text[50];
    ShowLabel(vbox, " ");

    sprintf(text, "计算机名\t： %s", oinfo.hostname);
    ShowLabel(vbox, text);

    sprintf(text, "系统名\t\t：%s", oinfo.os);
    ShowLabel(vbox, text);

    sprintf(text, "系统版本号\t：%s", oinfo.version);
    ShowLabel(vbox, text);

    sprintf(text, "内核版本号\t：%s", oinfo.kernel);
    ShowLabel(vbox, text);

    sprintf(text, "CPU\t\t：%s", oinfo.cpu);
    ShowLabel(vbox, text);

    sprintf(text, "内存\t\t：%s", oinfo.memory);
    ShowLabel(vbox, text);

    ShowLabel(vbox, " ");
}


