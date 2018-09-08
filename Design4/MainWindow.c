/***********************************************************************
 * @file MainWindow.c
	 MAINWINDOW
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/
#include "MainWindow.h"
#include "menu.h"
#include "outline.h"
#include "cpu.h"
#include "memory.h"
#include "module.h"
#include "status.h"
#include "process.h"
#include "disk.h"

GtkWidget *window;

int main(int argc, char **argv)
{

	GtkWidget *main_vbox;

	gtk_init(&argc, &argv);
	// 主窗口设置
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "系统监视器");
	gtk_window_set_opacity(GTK_WINDOW(window), 0.90); // 设置透明度函数
	// 窗口大小
	gtk_window_set_default_size(GTK_WINDOW(window), 1000, 750);
	// 窗口初始位置
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	// 关闭信号链接
	g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// 创建一个主窗口纵向盒
	main_vbox = gtk_vbox_new(FALSE, 2);
	// 周围的间距
	gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 2);
	// 添加进入主窗口
	gtk_container_add(GTK_CONTAINER(window), main_vbox);

	// 创建菜单栏
	CreateMenuBar(main_vbox);

	/* 建立多标签页notebook */
	GtkWidget *notebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos((GtkNotebook *)notebook, GTK_POS_TOP); //位置
	gtk_notebook_set_show_border((GtkNotebook *)notebook, FALSE);
	/* 在纵向盒顶部添加该笔记本 */
	gtk_box_pack_start(GTK_BOX(main_vbox), notebook, TRUE, TRUE, 0);

	// 获取CPU基本信息
	GetCPUInfo();

	// 添加子页面
	CreateOutline(notebook);
	CreateCPU(notebook);
	CreateMemory(notebook);
	CreateProcess(notebook);
	CreateModule(notebook);
	CreateDisk(notebook);

	// 状态栏
	ShowStatus(main_vbox);

	// 全部show出来！
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}

/* 功能:      设置控件字体大小 
  * widget:    需要改变字体的控件 
  * size:      字体大小 
  * is_button: TRUE代表控件为按钮，FALSE为其它控件 
  */
void set_widget_font_size(GtkWidget *widget, int size, gboolean is_button)
{
	GtkWidget *labelChild;
	PangoFontDescription *font;
	gint fontSize = size;

	font = pango_font_description_from_string("Sans");						 //"Sans"字体名
	pango_font_description_set_size(font, fontSize * PANGO_SCALE); //设置字体大小
	pango_font_description_set_style(font, PANGO_STYLE_NORMAL);

	if (is_button)
	{
		labelChild = gtk_bin_get_child(GTK_BIN(widget)); //取出GtkButton里的label
	}
	else
	{
		labelChild = widget;
	}

	//设置label的字体，这样这个GtkButton上面显示的字体就变了
	gtk_widget_modify_font(GTK_WIDGET(labelChild), font);
	pango_font_description_free(font);
}