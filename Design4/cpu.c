/***********************************************************************
 * @file cpu.c
     CPU
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "cpu.h"

void CreateCPU(GtkWidget* notebook)
{

    GtkWidget *vbox;  
    GtkWidget *text_label;

    // 创建盒装容器并添加
    vbox = gtk_vbox_new(FALSE, 10);
    // gtk_container_add(GTK_CONTAINER(notebook), vbox);
    gtk_container_border_width(GTK_CONTAINER(vbox), 150);

    text_label = gtk_label_new("CPU");
    gtk_box_pack_start(GTK_BOX(vbox), text_label, FALSE, FALSE, 10);
    set_widget_font_size(text_label,16,FALSE);
    gtk_widget_show(text_label);

    /* 将该标签页加入到notebook中 */
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, gtk_label_new("CPU"));
}