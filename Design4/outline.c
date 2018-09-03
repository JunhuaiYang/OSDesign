/***********************************************************************
 * @file outline.c
     OUTLINE
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "outline.h"

void CreateOutline(GtkWidget* notebook)
{

    GtkWidget *vbox;  

    // 创建盒装容器并添加
    vbox = gtk_vbox_new(FALSE, 10);
    // gtk_container_add(GTK_CONTAINER(notebook), vbox);
    gtk_container_border_width(GTK_CONTAINER(vbox), 150);
    gtk_widget_show(vbox);

    GtkWidget *vbox2;  
    vbox2 = gtk_vbox_new(FALSE, 10);
    vbox2 = gtk_vbox_new(FALSE, 10);
    // gtk_container_add(GTK_CONTAINER(notebook), vbox);
    gtk_container_border_width(GTK_CONTAINER(vbox2), 150);
    gtk_widget_show(vbox2);

    GtkWidget *text_label;
    
    text_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), text_label, FALSE, FALSE, 10);
    gtk_label_set_markup(
        GTK_LABEL(text_label), "<span font_desc='18'>0-9的循环：</span>");
    gtk_widget_show(text_label);

    /* 将该标签页加入到notebook中 */
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, gtk_label_new(
			"CPU信息"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox2, gtk_label_new(
			"进程信息"));
}

