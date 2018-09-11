/***********************************************************************
 * @file module.c
     MODULE
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "module.h"

GtkListStore *module_store;
GtkWidget *mtree_view;

void CreateModule(GtkWidget *notebook)
{

    GtkWidget *vbox;
    GtkWidget *scrolled_window;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    int i;

    char *name[M_COLUMN] = {"模块名", "占用的内存数量", "使用计数", "状态"};
    int min_width[M_COLUMNS] = {250, 200, 150, 200};

    // 创建盒装容器并添加
    vbox = gtk_vbox_new(FALSE, 0);
    // gtk_container_add(GTK_CONTAINER(notebook), vbox);
    gtk_container_border_width(GTK_CONTAINER(vbox), 5);

    /* 将该标签页加入到notebook中 */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, gtk_label_new("模块"));

    scrolled_window = gtk_scrolled_window_new(NULL, NULL); //添加滚动窗口控件
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_widget_show(scrolled_window);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    //添加list控件
    module_store = gtk_list_store_new(M_COLUMNS,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING
                                       );

    //添加tree view控件,显示进程信息树
    mtree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(module_store));
    g_object_unref(G_OBJECT(module_store));                       //减少引用次数
    gtk_container_add(GTK_CONTAINER(scrolled_window), mtree_view); //Tree in scrolled_window
    // 悬浮菜单
    gtk_tree_view_set_hover_selection(GTK_TREE_VIEW(mtree_view), FALSE);
    // 右键弹出菜单连接
    // g_signal_connect(G_OBJECT(mtree_view), "button-release-event", G_CALLBACK(PopMenu), NULL);
    // 显示列
    for (i = 0; i < M_COLUMN; i++)
    {
        renderer = gtk_cell_renderer_text_new(); //添加一个cell_renderer_text用于显示文字
        column = gtk_tree_view_column_new_with_attributes(name[i],
                                                          renderer, "text", i, NULL); //新建一列
        gtk_tree_view_append_column(GTK_TREE_VIEW(mtree_view), column);               //将该列加到树中
        // 改变宽度
        gtk_tree_view_column_set_min_width(column, min_width[i]);
    }


}