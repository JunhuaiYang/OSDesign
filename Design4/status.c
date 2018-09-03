/***********************************************************************
 * @file status.c
     STATUS
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "status.h"

void ShowStatus(GtkWidget* vbox)
{
  GtkWidget* statusbar = gtk_statusbar_new();
	//把工具栏加入盒子容器
	gtk_box_pack_end(GTK_BOX(vbox),statusbar,FALSE,TRUE,1);
	gtk_statusbar_push(GTK_STATUSBAR(statusbar), 0, "公司付款后");

}