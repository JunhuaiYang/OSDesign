#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <gtk/gtk.h>

gint ShowNum(gpointer label);

int main(int argc, char *argv[])
{
    //初始化GTK
    GtkWidget *window;
    GtkWidget *vbox;  
    GtkWidget *label;
    GtkWidget *text_label;

    gpointer data;

    // 初始化整个GTK
    gtk_init(&argc, &argv);
    // 生成窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // 窗口属性
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "累加");
    gtk_container_set_border_width(GTK_CONTAINER(window), 20); //边界宽度
    // gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    g_signal_connect_swapped(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit), NULL);
    // g_signal_connect(G_OBJECT(window), "destory", G_CALLBACK(destory_progress), NULL);
    
    // 创建盒装容器并添加
    vbox = gtk_vbox_new(FALSE, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_container_border_width(GTK_CONTAINER(vbox), 150);
    gtk_widget_show(vbox);

    // 标签
    text_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), text_label, FALSE, FALSE, 10);
    label = gtk_label_new("Number = 0");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 10);
    gtk_label_set_markup(
        GTK_LABEL(text_label), "<span font_desc='18'>0-9的循环：</span>");
    gtk_widget_show(text_label);
    gtk_widget_show(label);

    // 显示窗口
    gtk_widget_show(window);
    // 延时1000ms 循环调用函数
    g_timeout_add(1000, ShowNum, (gpointer)label);

    gtk_main();
}

gint ShowNum(gpointer label)
{
    int static count = 1;
    char string[100];
  
    sprintf(string, "Number = %d", count++);
    gtk_label_set_text(label, string);

    if(count == 10)
      count = 0;
}