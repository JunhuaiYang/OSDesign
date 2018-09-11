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
                                      G_TYPE_STRING);

    //添加tree view控件,显示进程信息树
    mtree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(module_store));
    g_object_unref(G_OBJECT(module_store));                        //减少引用次数
    gtk_container_add(GTK_CONTAINER(scrolled_window), mtree_view); //Tree in scrolled_window
    // 悬浮菜单
    gtk_tree_view_set_hover_selection(GTK_TREE_VIEW(mtree_view), FALSE);
    // 右键弹出菜单连接
    g_signal_connect(G_OBJECT(mtree_view), "button-release-event", G_CALLBACK(PopModuleMenu), NULL);
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

    GetModulesInfo(module_store);
}

void GetModulesInfo(GtkListStore *store)
{
    GtkTreeIter iter;

    char name[50];
    long int page;
    long int m_count;
    char status[30];
    char memory[20];
    char nouse[80];

    char count_str[10];

    FILE *fp = fopen("/proc/modules", "r");

    while ((fscanf(fp, "%s %ld %ld %s %s %s", name, &page, &m_count, nouse, status, nouse)) != -1)
    {
        sprintf(memory, "%.2fM", (float)page * getpagesize() / 1024.0 / 1024.0);
        sprintf(count_str, "%ld次", m_count);

        gtk_list_store_append(store, &iter); //增加到列表
        gtk_list_store_set(store, &iter,
                           M_NAME_COLUMN, name,
                           M_MEMORY_COLUMN, memory,
                           M_COUNT_COLUMN, count_str,
                           M_STATUS_COLUMN, status,
                           -1);
    }
    fclose(fp);
}

gboolean PopModuleMenu(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    GtkWidget *menu;
    GtkWidget *refresh, *destroy, *new_module;

    if (event->type == GDK_BUTTON_RELEASE && event->button == 3)
    {
        menu = gtk_menu_new();
        refresh = gtk_menu_item_new_with_label("刷新");
        g_signal_connect(G_OBJECT(refresh), "activate", G_CALLBACK(ModuleRefresh), widget);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), refresh);
        new_module = gtk_menu_item_new_with_label("新增模块(需要管理员权限)");
        g_signal_connect(G_OBJECT(new_module), "activate", G_CALLBACK(ModuleNew), widget);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), new_module);
        destroy = gtk_menu_item_new_with_label("卸载模块(需要管理员权限)");
        g_signal_connect(G_OBJECT(destroy), "activate", G_CALLBACK(ModuleDestory), widget);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), destroy);
        gtk_widget_show_all(GTK_WIDGET(menu));
        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event->button, event->time);
        return TRUE;
    }
    return FALSE;
}

gboolean ModuleRefresh(gpointer data)
{
    gtk_list_store_clear(module_store);
    GetModulesInfo(module_store);

    return TRUE;
}

gboolean ModuleNew(gpointer data)
{
    GtkWidget *filew;
    filew = gtk_file_selection_new("文件选择");

    gtk_widget_show(filew);
    g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filew)->ok_button), "clicked", G_CALLBACK(NewModule), filew);
    g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filew)->cancel_button), "clicked", G_CALLBACK(DestroyFs), filew);

    return TRUE;
}

void NewModule(GtkWidget *w, GtkFileSelection *fs)
{
    char command[50];
    const char *dir = gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs));
    int result;

    sprintf(command, "insmod %s\n", dir);
    result = system(command);
    if (result != 0)
    {
        ShowErrorDialog("模块加载失败！");
    }
    else
        ShowInfoDialog("模块加载成功！");

    gtk_widget_destroy(GTK_WIDGET(fs));
    ModuleRefresh(NULL);
}

void DestroyFs(GtkWidget *w, GtkFileSelection *fs)
{
    gtk_widget_destroy(GTK_WIDGET(fs));
}

gboolean ModuleDestory(gpointer data)
{
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkWidget *dialog; //声明一个对话框

    char *module_name;
    char command[50];
    int result;

    // 获得当前模块
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(mtree_view));
    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gtk_tree_model_get(model, &iter, M_NAME_COLUMN, &module_name, -1); //获得模块名

        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "你确定要卸载该模块吗？");
        gtk_window_set_title(GTK_WINDOW(dialog), "卸载模块");
        result = gtk_dialog_run(GTK_DIALOG(dialog));

        if (result == GTK_RESPONSE_YES)
        {
            sprintf(command, "rmmod %s", module_name);
            result = system(command);
            if (result != 0)
            {
                ShowErrorDialog("模块卸载失败！");
            }
            else
                ShowInfoDialog("模块卸载成功！");
        }
        gtk_widget_destroy(dialog); //删除对话框
        ModuleRefresh(NULL);

        return TRUE;
    }
    return FALSE;
}