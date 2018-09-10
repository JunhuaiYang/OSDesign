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

void CreateProcess(GtkWidget *notebook)
{

    GtkWidget *vbox;
    GtkWidget *scrolled_window;
    GtkListStore *process_store;
    GtkWidget *ptree_view;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    int i;

    char *name[P_COLUMN] = {"PID", "进程名", "状态", "CPU", "占用内存", "优先级"};

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
    char priority[5];
    char noneed[20];


    long int utime_now, stime_now, cutime_now, cstime_now;
    long int p_cpu_time, cpu_all;
    int page;
    float p_ratio;
    int num; //用于遍历
    int i;

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
            sscanf(buffer,"%s (%s %c %s %s %s %s %s %s %s %s %s %s %ld %ld %ld %ld %s %s %s %s %ld",
                    pid, process_name, &status, noneed, noneed, noneed, noneed, noneed, noneed, noneed,
                     noneed, noneed, noneed, &utime_now, &stime_now, &cutime_now, &cstime_now, noneed, 
                    priority,  noneed, noneed, noneed, noneed, &page  );

        }
        process_name[strlen(process_name)-1] = '\0';
        
        printf("%s %s\n",pid, process_name);
        // 状态转换
        // if (!strcmp(info[2], "S"))
        //     strcpy(process_status, "睡眠中");
        // else if (!strcmp(info[2], "R"))
        //     strcpy(process_status, "运行中");
        // else if (!strcmp(info[2], "T"))
        //     strcpy(process_status, "已停止");
        // else if (!strcmp(info[2], "D"))
        //     strcpy(process_status, "不可中断");
        // else if (!strcmp(info[2], "Z"))
        //     strcpy(process_status, "死锁中");

        // 获得cpu时间 4个cpu时间在13-16项中
        // utime_now = atoi(info[13]);
        // stime_now = atoi(info[14]);
        // cutime_now = atoi(info[15]);
        // cstime_now = atoi(info[16]);
        // sscanf(info[13], "%ld", &utime_now);
        // sscanf(info[14], "%ld", &stime_now);
        // if(info[15] != NULL)
        // sscanf(info[15], "%ld", &cutime_now);
        // else
        //     printf("%d", i);
        // sscanf(info[16], "%ld", &cstime_now);

        // 计算CPU使用率
        // △（p_cpu_time) / △all
        // p_cpu_time = utime_now+stime_now+cutime_now+cstime_now;

        // p_ratio = (float) (p_cpu_time - p_cpu_time_o[num]) /
        //                 (cpu_all - cpu_all_o);

        sprintf(ratio, "%.2f%%", 100 * p_ratio);

        // 23项为 驻留内存 page
        // 使用getpagesize获得系统内存页
        sprintf(memo_size, "%.2fM", (float)page * getpagesize() / 1024.0 / 1024.0);

        // 17 为动态优先级， 18为静态优先级

        //     gtk_list_store_append (store, &iter);//增加到列表
        //     gtk_list_store_set (store, &iter,
        //                             PID_COLUMN,info[0],
        //                             NAME_COLUMN,process_name,
        //                             STATUS_COLUMN,process_status,
        //                             CPU_COLUMN,ratio,
        //                             MEMORY_COLUMN,memo_size,
        //                             PRIORITY_COLUMN, info[18],
        //                             -1);

        // p_cpu_time_o[num] = p_cpu_time;
        // num =(num+1) % MAX_P;
    }
    cpu_all_o = cpu_all;
}
