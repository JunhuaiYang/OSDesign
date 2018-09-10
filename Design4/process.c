/***********************************************************************
 * @file process.c
     PROCESS
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "process.h"

long int p_cpu_time_o[MAX_P];
long int cpu_all_o;
GtkListStore *process_store;
GtkWidget *ptree_view;
pid_t g_pid;

void CreateProcess(GtkWidget *notebook)
{

    GtkWidget *vbox;
    GtkWidget *scrolled_window;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    int i;

    char *name[P_COLUMN] = {"PID", "进程名", "状态", "CPU", "驻留内存", "优先级"};
    int min_width[P_COLUMN] = {100, 300, 150, 100, 150, 100};

    // 创建盒装容器并添加
    vbox = gtk_vbox_new(FALSE, 3);
    gtk_container_border_width(GTK_CONTAINER(vbox), 2);

    /* 将该标签页加入到notebook中 */
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, gtk_label_new("进程"));

    scrolled_window = gtk_scrolled_window_new(NULL, NULL); //添加滚动窗口控件
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_widget_show(scrolled_window);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    //添加list控件
    process_store = gtk_list_store_new(NP_COLUMNS,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING,
                                       G_TYPE_STRING);

    //添加tree view控件,显示进程信息树
    ptree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(process_store));
    g_object_unref(G_OBJECT(process_store));                       //减少引用次数
    gtk_container_add(GTK_CONTAINER(scrolled_window), ptree_view); //Tree in scrolled_window
    // 悬浮菜单
    gtk_tree_view_set_hover_selection(GTK_TREE_VIEW(ptree_view), FALSE);
    // 右键弹出菜单连接
    g_signal_connect(G_OBJECT(ptree_view), "button-release-event", G_CALLBACK(PopMenu), NULL);
    // 显示列
    for (i = 0; i < P_COLUMN; i++)
    {
        renderer = gtk_cell_renderer_text_new(); //添加一个cell_renderer_text用于显示文字
        column = gtk_tree_view_column_new_with_attributes(name[i],
                                                          renderer, "text", i, NULL); //新建一列
        gtk_tree_view_append_column(GTK_TREE_VIEW(ptree_view), column);               //将该列加到树中
        // 改变宽度
        gtk_tree_view_column_set_min_width(column, min_width[i]);
    }

    GetStat(&stat_info);
    GetProcessInfo(process_store);
    g_timeout_add(5000, UpdateProcess, NULL);
}

void GetProcessInfo(GtkListStore *store)
{
    DIR *dir;
    int fd;
    GtkTreeIter iter;
    struct dirent *entry;
    char dir_buf[256];
    char buffer[1024];

    char process_status[30];
    char memo_size[20];
    char ratio[20];
    char pid[10];
    char process_name[50];
    char status;
    int priority;
    char priority_str[10];
    char noneed[20];

    long int utime_now, stime_now, cutime_now, cstime_now;
    long int p_cpu_time, cpu_all;
    long int page;
    float p_ratio;
    int num = 0; //用于遍历

    //用opendir打开一个与给定目录名相对应的目录流
    dir = opendir("/proc");

    cpu_all = stat_info.user + stat_info.nice + stat_info.sys +
              stat_info.idle + stat_info.iowait + stat_info.irq +
              stat_info.softirq;

    while ((entry = readdir(dir)) != NULL)
    {
        // 筛选名字为数字的文件夹
        if ((entry->d_name[0] >= '0') && (entry->d_name[0] <= '9'))
        {
            // 生成文件名
            sprintf(dir_buf, "/proc/%s/stat", entry->d_name);
            fd = open(dir_buf, O_RDONLY); //只读打开
            read(fd, buffer, sizeof(buffer));
            close(fd);

            // 14-17cpu 19有先 24内存
            sscanf(buffer, "%s (%s %c %s %s %s %s %s %s %s %s %s %s %ld %ld %ld %ld %s %d %s %s %s %s %ld",
                   pid, process_name, &status, noneed, noneed, noneed, noneed, noneed, noneed, noneed,
                   noneed, noneed, noneed, &utime_now, &stime_now, &cutime_now, &cstime_now, noneed,
                   &priority, noneed, noneed, noneed, noneed, &page);

            process_name[strlen(process_name) - 1] = '\0';

            // printf("%s %s %c %d %d %d %d %s %d \n",pid, process_name, status, utime_now, stime_now, cutime_now, cstime_now, priority, page);
            // 状态转换
            if (status == 'S')
                strcpy(process_status, "睡眠中");
            else if (status == 'R')
                strcpy(process_status, "运行中");
            else if (status == 'T')
                strcpy(process_status, "已停止");
            else if (status == 'D')
                strcpy(process_status, "不可中断");
            else if (status == 'Z')
                strcpy(process_status, "死锁中");
            else if (status == 'I')
                strcpy(process_status, "I");

            // 计算CPU使用率
            // △（p_cpu_time) / △all
            p_cpu_time = utime_now + stime_now + cutime_now + cstime_now;

            p_ratio = (float)(p_cpu_time - p_cpu_time_o[num]) /
                      (cpu_all - cpu_all_o);

            // printf("%d : %d %d\n", num, p_cpu_time,cpu_all );
            sprintf(ratio, "%.2f %%", 100.0 * p_ratio);

            // 23项为 驻留内存 page
            // 使用getpagesize获得系统内存页
            sprintf(memo_size, "%.2fM", (float)page * getpagesize() / 1024.0 / 1024.0);
            // printf("%d : %s %s\n", num, ratio, memo_size);

            // 17 为动态优先级priority， 18为静态优先级nice

            // 我们取priority 为nice
            if (priority <= -7)
            {
                strcpy(priority_str, "非常高");
            }
            else if (priority >= 7)
            {
                strcpy(priority_str, "非常低");
            }
            else if (priority < 7 && priority >= 3)
            {
                strcpy(priority_str, "低");
            }
            else if (priority > -7 && priority <= -3)
            {
                strcpy(priority_str, "高");
            }
            else
            {
                strcpy(priority_str, "普通");
            }

            gtk_list_store_append(store, &iter); //增加到列表
            gtk_list_store_set(store, &iter,
                               PID_COLUMN, pid,
                               NAME_COLUMN, process_name,
                               STATUS_COLUMN, process_status,
                               CPU_COLUMN, ratio,
                               MEMORY_COLUMN, memo_size,
                               PRIORITY_COLUMN, priority_str,
                               -1);

            p_cpu_time_o[num] = p_cpu_time;
            num = (num + 1) % MAX_P;
        }
    }
    cpu_all_o = cpu_all;
    closedir(dir);
}

gboolean UpdateProcess(gpointer data)
{
    gtk_list_store_clear(process_store); //Clear treeview
    GetProcessInfo(process_store);       //Re-read Process_info

    return TRUE;
}

gboolean PopMenu(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    GtkWidget *menu;
    GtkWidget *kill, *pause_p, *continue_p;
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    char *pid;
    // 单击 3为右键
    if (event->type == GDK_BUTTON_RELEASE && event->button == 3)
    {
        // 先获得该进程pid
        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptree_view));
        if (gtk_tree_selection_get_selected(selection, &model, &iter))
        {
            gtk_tree_model_get(model, &iter, PID_COLUMN, &pid, -1); //在书的相应列中获得该进程的PID
            g_pid = atoi(pid);                                      //字符串转换成长整数
        }

        menu = gtk_menu_new();
        kill = gtk_menu_item_new_with_label("杀死");
        g_signal_connect(G_OBJECT(kill), "activate", G_CALLBACK(KillProcess), widget);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), kill);
        pause_p = gtk_menu_item_new_with_label("暂停");
        g_signal_connect(G_OBJECT(pause_p), "activate", G_CALLBACK(StopProcess), widget);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), pause_p);
        continue_p = gtk_menu_item_new_with_label("继续");
        g_signal_connect(G_OBJECT(continue_p), "activate", G_CALLBACK(ContinueProcess), widget);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), continue_p);
        gtk_widget_show_all(GTK_WIDGET(menu));
        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event->button, event->time);
        return TRUE;
    }
    return FALSE;
}

gboolean KillProcess(gpointer data)
{
    int result;
    GtkWidget *dialog; //声明一个对话框

    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "你确定要杀死该进程吗？");
    gtk_window_set_title(GTK_WINDOW(dialog), "杀死进程");
    result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_YES)
    {
        // 向进程发送SIGTERM信号，正常结束
        if (kill(g_pid, SIGTERM) == -1)
        {
            // 失败
            ShowErrorDialog("结束进程失败！");
        }
        else
        {
            ShowInfoDialog("结束进程成功！");
        }
    }
    gtk_widget_destroy(dialog); //删除对话框
    // 刷新
    UpdateProcess(NULL);

    return TRUE;
}

gboolean StopProcess(gpointer data)
{
    int result;
    GtkWidget *dialog; //声明一个对话框

    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "你确定要暂停该进程吗？");
    gtk_window_set_title(GTK_WINDOW(dialog), "暂停进程");
    result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_YES)
    {
        // 向进程发送SIGSTOP信号，暂停
        if (kill(g_pid, SIGSTOP) == -1)
        {
            // 失败
            ShowErrorDialog("进程暂停失败！");
        }
        else
        {
            ShowInfoDialog("进程暂停成功！");
        }
    }
    gtk_widget_destroy(dialog); //删除对话框
    // 刷新
    UpdateProcess(NULL);

    return TRUE;
}

gboolean ContinueProcess(gpointer data)
{
    int result;
    GtkWidget *dialog; //声明一个对话框

    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "你确定要恢复该进程吗？");
    gtk_window_set_title(GTK_WINDOW(dialog), "恢复进程");
    result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_YES)
    {
        // 向进程发送SIGCONT信号，继续
        if (kill(g_pid, SIGCONT) == -1)
        {
            // 失败
            ShowErrorDialog("进程继续运行失败！");
        }
        else
        {
            ShowInfoDialog("进程继续运行成功！");
        }
    }
    gtk_widget_destroy(dialog); //删除对话框
    // 刷新
    UpdateProcess(NULL);

    return TRUE;
}

void ShowErrorDialog(char *info)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", info);
    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void ShowInfoDialog(char *info)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO,
                                    GTK_BUTTONS_OK, "%s", info);

    gtk_window_set_title(GTK_WINDOW(dialog), "信息");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}