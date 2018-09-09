/***********************************************************************
 * @file process.c
     PROCESS
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "process.h"

void CreateProcess(GtkWidget *notebook)
{

    GtkWidget *vbox;
    GtkWidget *scrolled_window;
    GtkWidget *hbox;
    GtkListStore *process_store;
    GtkWidget *ptree_view;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    int i;

    char *name[P_COLUMN] = {"PID", "进程名", "状态", "CPU", "占用内存"};

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
    g_object_unref(G_OBJECT(process_store)); //减少引用次数
    gtk_widget_show(ptree_view);
    gtk_container_add(GTK_CONTAINER(scrolled_window), ptree_view); //Tree in scrolled_window

    // 显示列
    for (i = 0; i < P_COLUMN; i++)
    {
        renderer = gtk_cell_renderer_text_new(); //添加一个cell_renderer_text用于显示文字
        column = gtk_tree_view_column_new_with_attributes(name[i],
                                                          renderer, "text", i, NULL); //新建一列
        gtk_tree_view_append_column(GTK_TREE_VIEW(ptree_view), column);               //将该列加到树中
    }

    GetProcessInfo(process_store);
}

void GetInfo(char *string, char **info)
{
    int i;
    char *delim = " ";               //分隔符 一个空格
    info[0] = strtok(string, delim); //以delim分隔符切割string
    printf("%d:%s\n",0, info[0]);
    // 总共有52项
    for (i = 1; i < 50; i++)
    {
        // 第二次调用strtok要传NULL
        info[i] = strtok(NULL, delim); 
        printf("%d:%s\n",i, info[i]);
    }
}

void GetProcessInfo(GtkListStore *store)
{
    DIR *dir;
    int fd, i,num = 0;
    GtkTreeIter iter;
    struct dirent *entry;
    char dir_buf[256];
    char buffer[1024];
    char *info[50];
    char process_name[50];
    char process_status[30];


    //用opendir打开一个与给定目录名相对应的目录流
    dir = opendir ("/proc");
    while ((entry = readdir (dir)) != NULL )
    {
        // 筛选名字为数字的文件夹
        if ((entry->d_name[0] >= '0') && (entry->d_name[0] <= '9'))
        {
            // 生成文件名
            sprintf (dir_buf, "/proc/%s/stat", entry->d_name);
            fd = open (dir_buf, O_RDONLY);//只读打开
            read (fd, buffer, sizeof (buffer));
            close (fd);

            GetInfo(buffer, info);
        }
        sscanf(info[1], "(%s)", process_name);
        
        // 状态转换
        if (!strcmp(buf[2], "S"))
			strcpy(process_status, "睡眠中");
		else if (!strcmp(buf[2], "R"))
			strcpy(process_status, "运行中");
		else if (!strcmp(buf[2], "T"))
			strcpy(process_status, "已停止");
		else if (!strcmp(buf[2], "D"))
			strcpy(process_status, "不可中断");
		else if (!strcmp(buf[2], "Z"))
			strcpy(process_status, "死锁中");


    }
}
