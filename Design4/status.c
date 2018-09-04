/***********************************************************************
 * @file status.c
     STATUS
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "status.h"

void ShowStatus(GtkWidget *vbox)
{
  GtkWidget *label;

  label = gtk_label_new("当前时间为");
  gtk_box_pack_end(GTK_BOX(vbox), label, FALSE, TRUE, 1);

  // 延时1000ms 循环调用函数
  g_timeout_add(1000, GetTime, (gpointer)label);
}

gint GetTime(gpointer label)
{
  time_t t;
  time(&t); //获得系统时间
  char string[100];
  sprintf(string, "%s", ctime(&t));
  string[strlen(string)-1] = '\0';  //去掉换行符
  gtk_label_set_text(label, string);

  return TRUE;
}

