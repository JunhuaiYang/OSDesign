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
  GtkWidget *status_bar;

  // 创建状态栏
  status_bar = gtk_statusbar_new();
  gtk_box_pack_start(GTK_BOX(vbox), status_bar, TRUE, TRUE, 0);
  gtk_statusbar_push(GTK_STATUSBAR(status_bar), 0, "buff");


}