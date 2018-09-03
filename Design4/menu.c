/***********************************************************************
 * @file menu.c
     MENU
 * @brief   header file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "menu.h"

int CreateMenuBar(GtkWidget *vbox)
{
  GtkWidget *menu_bar;
  GtkWidget *file_menu;
  GtkWidget *edit_menu;
  GtkWidget *help_menu;

  GtkWidget *file;
  GtkWidget *quit;

  GtkWidget *edit;
  GtkWidget *update;
  GtkWidget *pauseit;

  GtkWidget *help;
  GtkWidget *helpit;
  GtkWidget *about;

  char buf[128];

  // 菜单栏
  menu_bar = gtk_menu_bar_new();
  file_menu = gtk_menu_new();
  edit_menu = gtk_menu_new();
  help_menu = gtk_menu_new();

  // file_menu
  file = gtk_menu_item_new_with_label("文件");
  quit = gtk_menu_item_new_with_label("退出");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), file_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);

  // edit_menu
  edit = gtk_menu_item_new_with_label("编辑");
  update = gtk_menu_item_new_with_label("刷新");
  pauseit = gtk_menu_item_new_with_label("暂停");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), edit_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), update);
  gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), pauseit);  
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), edit);

  // help_menu
  help = gtk_menu_item_new_with_label("帮助");
  helpit = gtk_menu_item_new_with_label("使用说明");
  about = gtk_menu_item_new_with_label("关于");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), help_menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), helpit);
  gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help);

  // 菜单栏运行
  gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 3);

  // 信号处理
  g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(gtk_main_quit), NULL);



	// gtk_widget_show(menu_bar);
  // gtk_widget_show(file_menu);
  // gtk_widget_show(file);
  // gtk_widget_show(quit);

}