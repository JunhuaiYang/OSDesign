/***********************************************************************
 * @file process.c
     PROCESS
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "process.h"

void CreateProcess(GtkWidget* notebook)
{

    GtkWidget *vbox;  
    GtkWidget *scrolled_window;

    // 创建盒装容器并添加
    vbox = gtk_vbox_new(FALSE, 3);
    // gtk_container_add(GTK_CONTAINER(notebook), vbox);
    gtk_container_border_width(GTK_CONTAINER(vbox), 2);

    /* 将该标签页加入到notebook中 */
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, gtk_label_new("进程"));

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);//添加滚动窗口控件
    // gtk_widget_set_size_request (scrolled_window, 300, 300);
    gtk_widget_show (scrolled_window);
    gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);

    
}
