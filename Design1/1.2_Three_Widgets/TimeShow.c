#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <gtk/gtk.h>

gint GetTime(gpointer label);

int main(int argc, char *argv[])
{
    time_t t;
    time(&t);

    //初始化GTK
    GtkWidget *window;
    GtkWidget *vbox;  
    GtkWidget *label;
    GtkWidget *text_label;

    // 初始化整个GTK
    gtk_init(&argc, &argv);
    // 生成窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // 窗口属性
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "时间显示");
    gtk_container_set_border_width(GTK_CONTAINER(window), 20); //边界宽度
    // gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    g_signal_connect_swapped(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit), NULL);
    
    // 创建盒装容器并添加
    vbox = gtk_vbox_new(FALSE, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_container_border_width(GTK_CONTAINER(vbox), 150);
    gtk_widget_show(vbox);

    // 标签
    text_label = gtk_label_new("当前时间为");
    gtk_box_pack_start(GTK_BOX(vbox), text_label, FALSE, FALSE, 10);
    label = gtk_label_new(ctime(&t));
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 10);
    gtk_label_set_markup(
        GTK_LABEL(text_label), "<span font_desc='18'>现在时间是</span>");
    gtk_widget_show(text_label);
    gtk_widget_show(label);

    // 显示窗口
    gtk_widget_show(window);
    // 延时1000ms 循环调用函数
    g_timeout_add(1000, GetTime, (gpointer)label);

    gtk_main();
}

gint GetTime(gpointer label)
{
    time_t t;
    time(&t); //获得系统时间
    char string[100];
    sprintf(string, "%s", ctime(&t));
    gtk_label_set_text(label, string);
}