/***********************************************************************
 * @file MainWindow.c
	 MAINWINDOW
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/
#include "MainWindow.h"

int main(int argc, char ** argv)
{

	GtkWidget * window;
	GtkWidget * main_vbox;
	GtkWidget *tooltip;
	gtk_init(&argc, &argv);

	// 主窗口
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "系统监视器");
	gtk_window_set_opacity(GTK_WINDOW(window), 0.9); // 设置透明度函数
	// 窗口大小
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	// 窗口初始位置
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_show(window);
	// 关闭信号
    g_signal_connect_swapped(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit), NULL);

	// 创建一个纵向盒
	// gint spacing为盒子之间间距
	main_vbox = gtk_vbox_new(FALSE, 10);
	// 周围的间距 
	gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 5);
	// 添加
	gtk_container_add(GTK_CONTAINER(window), main_vbox);
	gtk_widget_show(main_vbox);

	// createMenuBar(main_vbox);

	/* 建立多标签页notebook */
	GtkWidget *notebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos((GtkNotebook *) notebook, GTK_POS_TOP); //位置
	gtk_notebook_set_show_border((GtkNotebook *) notebook, FALSE);
	/* 在纵向盒顶部添加该笔记本 */
	gtk_box_pack_end(GTK_BOX(main_vbox), notebook, TRUE, TRUE, 0);

	/* 显示该笔记本 */
	gtk_widget_show(notebook);

	// /* 新建第一个标签页到notebook，用的是frame框架 */
	// createCPUPage(notebook);

	// /* 新建第三个标签页到notebook */
	// createModPage(notebook);

	// /* 新建第四个标签页到notebook */
	// createProPage(notebook);

	// /* 新建第五个标签页到notebook */
	// /* 原来是mem模块在死循环中 */
	// createMemPage(notebook);

	// /* 新建第二个标签页到notebook */
	// createAboutPage(notebook);

	// gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
	// /**************************************************/
	// gtk_timeout_add(400, refresh, pdata);

	gtk_main();
	return 0;
}
