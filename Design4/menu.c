/***********************************************************************
 * @file menu.c
     MENU
 * @brief   header file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "menu.h"

void CreateMenuBar(GtkWidget *vbox)
{
    GtkWidget *menu_bar;
    GtkWidget *file_menu;
    GtkWidget *edit_menu;
    GtkWidget *help_menu;

    GtkWidget *file;
    GtkWidget *quit;
    GtkWidget *shutdown;
    GtkWidget *reboot;


    GtkWidget *edit;
    GtkWidget *update;
    GtkWidget *pauseit;
    GtkWidget *pcontinue;

    GtkWidget *help;
    GtkWidget *helpit;
    GtkWidget *about;

    // 菜单栏
    menu_bar = gtk_menu_bar_new();
    file_menu = gtk_menu_new();
    edit_menu = gtk_menu_new();
    help_menu = gtk_menu_new();

    // file_menu
    file = gtk_menu_item_new_with_label("文件");
    quit = gtk_menu_item_new_with_label("退出");
    shutdown = gtk_menu_item_new_with_label("关机");
    reboot = gtk_menu_item_new_with_label("重启");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), shutdown);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), reboot);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);

    // edit_menu
    edit = gtk_menu_item_new_with_label("编辑");
    update = gtk_menu_item_new_with_label("刷新");
    pauseit = gtk_menu_item_new_with_label("暂停");
    pcontinue = gtk_menu_item_new_with_label("继续");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), edit_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), update);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), pauseit);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), pcontinue);
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
    g_signal_connect(G_OBJECT(shutdown), "activate", G_CALLBACK(MenuShutDown), NULL);
    g_signal_connect(G_OBJECT(reboot), "activate", G_CALLBACK(gtk_main_quit), NULL);
}


void MenuShutDown(GtkWidget *widget,gpointer data)
{

}

void MenuReboot(GtkWidget *widget,gpointer data)
{

}