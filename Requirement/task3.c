#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <sys/stat.h>
#include <gdk/gdkkeysyms.h>

#define DENSITY 100         //图形点密度
#define PMAX    100         //最大进程数目

enum
{
    NAME_COLUMN,  //进程名列
    PID_COLUMN,    //进程PID列
    STATUS_COLUMN, //进程状态列
    CPU_COLUMN,  //CPU信息列
    MEMORY_COLUMN,//内存信息列
    NP_COLUMNS
};

enum
{
    MOD_COLUMN,
    DEPEND_COLUMN,
    NM_COLUMNS
};


//Declare of GtkWidget
GtkWidget *vbox;//垂直布局控件
GtkWidget *hbox;//水平布局
GtkWidget *label;//标签
GtkWidget *frame;//框架
GtkWidget *window;//窗口
GtkWidget *menubar;//菜单栏
GtkWidget *notebook;//笔记本
GtkWidget *main_vbox;
GtkWidget *mem_label;
GtkWidget *swap_label;
GtkWidget *info_label;
GtkWidget *status_bar;
GtkWidget *ptree_view;
GtkWidget *mtree_view;
GtkWidget *cpu_draw_area;
GtkWidget *mem_draw_area;
GtkWidget *scrolled_window;
GtkWidget *prefresh_button , *pdelete_button; //Refresh & Kill Button

// Global Varities
gint cpu_graph[DENSITY];                    //CPU Graphic Density
gint mem_graph[DENSITY];                    //RAM Graphic Density
gdouble rate = 0;                           //cpu Use_Rate
gdouble total = 0;                          //当前进程总数
gdouble fuser = 0;                          //当前CPU使用信息
gdouble ftotal = 0;                         //当前CPU使用总数l
gdouble pfuser[PMAX];                  //No. MaxProcess
GString   *info;
GdkPixmap *cgraph = NULL;                   //pixmap_Pointer_CPU
GdkPixmap *mgraph = NULL;                   //pixmap_Pointer_RAM

//Others
GtkAccelGroup *accel_group;
GtkListStore *process_store;
GtkItemFactory *item_factory;
GtkCellRenderer *renderer;//用于显示tree view中的每个列标题
GtkTreeViewColumn *column;//tree view控件列数

//函数声明
void about (GtkWidget *, gpointer);
void destroy_window (GtkWidget *, gpointer);
void sys_shutdown(GtkWidget *, gpointer);
void sys_reboot (GtkWidget *, gpointer);
void sys_halt (GtkWidget *, gpointer);
void about_author (GtkWidget *, gpointer);

gboolean handle_timeout (gpointer data);
gboolean load_graph_refresh (GtkWidget *widget);
gboolean get_cpu_rate (gpointer data);

void create_sys_status_page (void);
void create_process_page (void);
void create_sys_info_page (void);

void get_status_info (void);
void get_process_info (GtkListStore *);
void get_cpu_info (GString *);
void get_os_info (GString *);

void draw_cpu_load_graph (void);
void draw_mem_load_graph (void);

void prefresh_button_clicked (gpointer data);
void pdelete_button_clicked (gpointer data);
void mrefresh_button_clicked (gpointer data);
void mdelete_button_clicked (gpointer data);

gboolean cpu_configure_event (GtkWidget *, GdkEventConfigure *, gpointer);
gboolean cpu_expose_event (GtkWidget *, GdkEventExpose *, gpointer);
gboolean mem_configure_event (GtkWidget *, GdkEventConfigure *, gpointer);
gboolean mem_expose_event (GtkWidget *, GdkEventExpose *, gpointer);

gchar ABt[]="About me ";
gchar ABc[]="     __<U201514876>Lizidong__     ";
void show_dialog (gchar *, gchar *);


static GtkItemFactoryEntry menu_items[] =
{
    {"/_File", NULL, NULL, 0, "<Branch>"},
    {"/File/Quit", "<CTRL>Q", destroy_window, 0, "<Item>"},
    {"/_Option", NULL, NULL, 0, "<Branch>"},
    {"/Option/Shutdown", "<CTRL>U", sys_shutdown, 0 ,"<Item>"},
    {"/Option/Reboot", "<CTRL>R", sys_reboot, 0 ,"<Item>"},
    {"/Option/Halt", "<CTRL>H",  sys_halt, 0 ,"<Item>"},
    {"/_About",NULL,NULL,0,"<Branch>"},
    {"/About/AboutUs","<CTRL>B", about, 0, "<Item>"},
};

static gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);//No.Menu

int main (int argc, char **argv)
{

    gtk_set_locale ();//Set LocalEnvir Vars
    gtk_init (&argc, &argv);//初始化

    memset (cpu_graph, 50, sizeof (cpu_graph));//Alloc 50size
    memset (mem_graph, 50, sizeof (mem_graph));//Alloc 50size
    memset (pfuser, 0 ,sizeof (pfuser));//pfuser的每个字节初始化为0

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);//创建主窗口,返回控件指针
    gtk_window_set_title (GTK_WINDOW (window), "Zidong Li's Linux System Moniter");//Title
    gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);//设置窗口大小
    gtk_window_set_resizable (GTK_WINDOW (window), FALSE);//设置窗口为不可伸缩
    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(destroy_window), NULL);//信号注册

    main_vbox =gtk_vbox_new (FALSE, 0);//创建主垂直布局控件,FALSE表示控件大小不一致,控件间隔
    gtk_widget_show (main_vbox);
    gtk_container_add (GTK_CONTAINER (window), main_vbox);//容器window添加垂直控件main_vbox
    accel_group = gtk_accel_group_new ();
    item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR,"<main>", accel_group);//Create Menu
    gtk_item_factory_create_items (item_factory, nmenu_items,menu_items, NULL);
    gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
    menubar = gtk_item_factory_get_widget (item_factory, "<main>");
    gtk_widget_show (menubar);
    gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, FALSE, 0);

    notebook = gtk_notebook_new ();//New a notebook
    gtk_widget_show (notebook);
    gtk_box_pack_start (GTK_BOX (main_vbox), notebook, FALSE, FALSE, 0);


    status_bar = gtk_statusbar_new ();//New a status_bar
    gtk_widget_show (status_bar);
    gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (status_bar),TRUE); //Can be Resized
    gtk_box_pack_start (GTK_BOX (main_vbox), status_bar, TRUE, FALSE, 0);


    gtk_timeout_add (2000, (GtkFunction)handle_timeout, NULL);
    gtk_timeout_add (1000, (GtkFunction)load_graph_refresh, NULL); //graph refresh per second
    gtk_timeout_add (1000, (GtkFunction)get_cpu_rate, NULL); //CPUrate refresh per second

    create_process_page ();
    create_sys_status_page ();
    create_sys_info_page ();

    gtk_widget_show_all (window);//显示窗口中的所有控件
    gtk_main ();//主事件循环,程序运行停在这里等待键鼠事件的发生,等待用户来操作窗口

    return 0;
}

/**************************回调函数************************/
void about(GtkWidget *widget, gpointer data)
{
    show_dialog(ABt,ABc);
}

void destroy_window (GtkWidget *widget, gpointer data)//退出窗口管理程序
{
    gtk_main_quit ();
}


void sys_shutdown (GtkWidget *widget, gpointer data)//Shutdown
{
    system ("shutdown -r now");
}

void sys_reboot (GtkWidget *widget, gpointer data)//注销
{
    system ("reboot");
}

void sys_halt (GtkWidget *widget, gpointer data)//Halt
{
    system ("halt");//停止系统运行,不关机
}




gboolean handle_timeout (gpointer data)//a series of operations by time
{
    gint page_num;

    page_num = gtk_notebook_get_current_page (GTK_NOTEBOOK (notebook));//Get Curr Page
    switch (page_num)
    {
    case 0:
        gtk_list_store_clear (process_store);//Clear treeview
        get_process_info (process_store);//Re-read Process_info
        break;
    default:
        break;
    }
    return TRUE;
}



gboolean load_graph_refresh (GtkWidget *widget)
{
    draw_cpu_load_graph ();
    draw_mem_load_graph ();
    return TRUE;
}


gboolean get_cpu_rate ( gpointer data)//Get CPU UseRate
{
    int fd,i;
    gchar buffer[256];
    gchar *cpu_time[9];
    gchar *tmp;
    gchar *delim =  " ";//分隔字符串
    gdouble cuser;
    gdouble ctotal;


    fd = open ("/proc/stat", O_RDONLY);//ReadFile
    read (fd, buffer, sizeof (buffer));
    close (fd);
    tmp = strstr (buffer, "cpu0");//Buffer ended with "cpu0"
    tmp--;
    *tmp = '\0';
    cpu_time[0] = strtok (buffer, delim);//split
    //strtok()函数用来将字符串分割成一个个片段
    for (i = 1; i < 9 ; i++)  cpu_time[i] = strtok (NULL, delim);
    cuser = atoi (cpu_time[1]);//Change str to long
    ctotal = (cuser + atoi (cpu_time[2]) + atoi (cpu_time[3]) + atoi (cpu_time[4]));
    total = ctotal - ftotal;
    rate = (cuser - fuser) / total;
    fuser = cuser;
    ftotal = ctotal;
    return TRUE;
}

/*************************创建页面****************************/
void create_sys_status_page ()//create_sys_status_page
{
    vbox = gtk_vbox_new (FALSE, 0);//添加纵向盒子
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (notebook), vbox);//Add it!

    frame = gtk_frame_new ("CPU");//添加用于显示CPU信息的边框构件
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);//加到盒子中

    cpu_draw_area = gtk_drawing_area_new ();//Add DrawPad
    gtk_widget_show (cpu_draw_area);
    gtk_drawing_area_size (GTK_DRAWING_AREA (cpu_draw_area), 400, 100);//Set DrawArea
    gtk_container_add (GTK_CONTAINER (frame), cpu_draw_area);

    g_signal_connect (cpu_draw_area, "expose_event",
                      G_CALLBACK (cpu_expose_event), NULL);
    g_signal_connect (cpu_draw_area, "configure_event",
                      G_CALLBACK (cpu_configure_event), NULL);
    //g_signal_connect()参数:发出信号的控件;信号名称;回调函数;传给回调函数的data


    frame = gtk_frame_new ("Memory");//添加用于显示内存信息的边框构件
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 10);

    mem_draw_area = gtk_drawing_area_new ();
    gtk_widget_show (mem_draw_area);
    gtk_drawing_area_size (GTK_DRAWING_AREA (mem_draw_area), 400, 100);//Set DrawArea
    gtk_container_add (GTK_CONTAINER (frame), mem_draw_area);
    g_signal_connect (mem_draw_area, "expose_event",
                      G_CALLBACK (mem_expose_event), NULL);
    g_signal_connect (mem_draw_area, "configure_event",
                      G_CALLBACK (mem_configure_event), NULL);


    mem_label = gtk_label_new ("");//RAM Label
    gtk_widget_show (mem_label);
    gtk_box_pack_start (GTK_BOX (vbox), mem_label, FALSE, FALSE, 0);

    swap_label = gtk_label_new ("");//Exchange Label
    gtk_widget_show (swap_label);
    gtk_box_pack_start (GTK_BOX (vbox), swap_label, FALSE, FALSE, 10);

    label = gtk_label_new ("Status");//Add Title
    gtk_widget_show (label);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
                                gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 1), label);
}

//显示进程页面信息
void create_process_page ()
{
    int i;
    gchar *col_name[5] = { "NAME", "PID" , "STATUS", "CPU" , "MEMORY"};

    vbox = gtk_vbox_new (FALSE, 0);//添加纵向盒子
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (notebook), vbox);

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);//添加滚动窗口控件
    gtk_widget_set_size_request (scrolled_window, 300, 300);
    gtk_widget_show (scrolled_window);
    gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);

    //进程信息
    process_store = gtk_list_store_new (NP_COLUMNS,
                                        G_TYPE_STRING,
                                        G_TYPE_STRING,
                                        G_TYPE_STRING,
                                        G_TYPE_STRING,
                                        G_TYPE_STRING);

    ptree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (process_store));
    //添加tree view控件,显示进程信息树

    g_object_unref (G_OBJECT (process_store));//减少引用次数

    gtk_widget_show (ptree_view);
    gtk_container_add (GTK_CONTAINER (scrolled_window), ptree_view);//Tree in scrolled_window
    //将进程信息树加到滚动窗口中

    for (i = 0; i < 5; i++)
    {
        renderer = gtk_cell_renderer_text_new ();//添加一个cell_renderer_text用于显示文字
        column = gtk_tree_view_column_new_with_attributes (col_name[i],
                 renderer,"text",i,NULL);//新建一列
        gtk_tree_view_append_column (GTK_TREE_VIEW (ptree_view), column);//将该列加到树中 
    }

    get_process_info (process_store); //获取进程信息并存取显示到列表

    hbox = gtk_hbox_new (FALSE, 0);//添加横排列盒子
    gtk_widget_show (hbox);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, FALSE, 0);//增加到竖排列盒子中

    prefresh_button = gtk_button_new ();//创建刷新进程信息按钮
    gtk_widget_show (prefresh_button);
    gtk_widget_set_size_request (prefresh_button, 70, 30);//Set Button Size
    gtk_button_set_label (GTK_BUTTON (prefresh_button), "refresh");//设置刷新按钮上的文字
    g_signal_connect (G_OBJECT (prefresh_button),"clicked",
                      G_CALLBACK(prefresh_button_clicked),
                      NULL);//刷新按钮点击后执行prefresh_button_clicked
    gtk_box_pack_start (GTK_BOX (hbox), prefresh_button, TRUE, FALSE, 0);//增加该按钮到横排列盒子中

    pdelete_button = gtk_button_new ();//创建杀死进程按钮
    gtk_widget_show (pdelete_button);
    gtk_widget_set_size_request (pdelete_button, 70, 30);
    gtk_button_set_label (GTK_BUTTON (pdelete_button), "kill");
    g_signal_connect (G_OBJECT (pdelete_button),"clicked",
                      G_CALLBACK(pdelete_button_clicked),
                      NULL);//该按钮点击后执行pdelete_button_clicked
    gtk_box_pack_start (GTK_BOX (hbox), pdelete_button, TRUE, FALSE, 0);//增加该按钮到横排列盒子中


    label = gtk_label_new ("Processes");
    gtk_widget_show (label);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
                                gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0), label);
}

//显示内核信息页面
void create_sys_info_page (void)//Add Page
{
    vbox = gtk_vbox_new (FALSE, 0);//添加纵向盒子
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER(notebook), vbox);

    frame = gtk_frame_new ("CPU Information:");
    gtk_widget_show (frame);//显示CPU信息边框
    gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 10);//加到纵向盒子里

    info = g_string_new ("");
    get_cpu_info (info);//用info变量存储获得的CPU信息
    info_label = gtk_label_new (info->str);//用标签info_label显示info变量的字符串内容
    gtk_widget_show (info_label);
    gtk_container_add (GTK_CONTAINER(frame), info_label);//将该标签加入边框中

    frame = gtk_frame_new ("OS Information:");//显示操作系统信息边框
    gtk_widget_show (frame);
    gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 30);

    info = g_string_new ("");
    get_os_info (info);//用info变量存储获得的操作系统信息
    info_label = gtk_label_new (info->str);//用标签info_label显示info变量的字符串内容
    gtk_widget_show (info_label);
    gtk_container_add (GTK_CONTAINER(frame), info_label);

    label = gtk_label_new ("SysInfo");
    gtk_widget_show (label);
    gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
                                gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook),2), label);
}

/**************以下为获取系统信息模块*****************/

void get_process_info (GtkListStore *store)//获取进程信息
{
    DIR *dir;
    int fd, i,num = 0;
    GtkTreeIter iter;
    struct dirent *entry;
    gdouble mem;
    gdouble srate;
    gdouble pcuser[PMAX];
    gchar *info[26];
    gchar buffer[128];
    gchar dir_buf[256];
    gchar *delim = " ";
    gchar mem_buffer[16];
    gchar rate_buffer[16];
    gchar state[2][10]= {"running", "sleeping"};

    //opendir打开一个与给定目录名相对应的目录流,返回指向该目录流的指针
    dir = opendir ("/proc");// 打开/proc文件目录,dir为返回指针
    while ((entry = readdir (dir)) != NULL )
    {
        if ((entry->d_name[0] >= '0') && (entry->d_name[0] <= '9'))
        {
            sprintf (dir_buf, "/proc/%s/stat", entry->d_name);//读文件到dir_buf中
            fd = open (dir_buf, O_RDONLY);//只读打开
            read (fd, buffer, sizeof (buffer));
            close (fd);

            //strtok用于分割字符串
            info[0] =  strtok (buffer, delim);//以delim分隔符切割buffer的一部分到info[0]
            for (i = 1; i < 26 ; i++){
            	     info[i] = strtok(NULL, delim);//查找整个buffer字符串
            }
            info[1]++;
            info[1] = strtok(info[1], ")");

            switch(info[2][0])
            {
            case 'R':
                info[2] = state[0];
                break;
            case 'S':
                info[2] = state[1];
                break;
            }

            pcuser[num] = atoi (info[13]);
            srate = (pcuser[num]-pfuser[num]) / (2 * total);//Percent Calc
            if(srate<0||srate>1)
            {
                srate=0;
            }
            sprintf (rate_buffer, "%.2f%%",100 * srate);

            mem = atoi (info[22]);//该进程的内存使用量
            mem = mem / (1024 * 1024);
            sprintf (mem_buffer, "%-.2f MB",mem);

            gtk_list_store_append (store, &iter);//增加到列表
            gtk_list_store_set (store, &iter,
                                NAME_COLUMN,info[1],
                                PID_COLUMN,info[0],
                                STATUS_COLUMN,info[2],
                                CPU_COLUMN,rate_buffer,
                                MEMORY_COLUMN,mem_buffer,
                                -1);
            pfuser[num] = pcuser[num];
            num = (num + 1 ) % PMAX;
        }
    }
    closedir (dir);
}



void get_cpu_info(GString *string){//获得cpu信息
       int fd,i;
       gchar buffer[1024];								
       gchar *delim = "\n";
       gchar *cpu_info[17];
       gchar *tmp;
       fd = open ("/proc/cpuinfo", O_RDONLY);//读取cpu相关信息的文件
       read (fd, buffer, sizeof (buffer));
       close (fd);
       cpu_info[0] = strtok (buffer, delim);
       for (i=1; i < 17 ; i++) {
		cpu_info[i] = strtok (NULL,delim);//分割字符串
       }	  
       for (i=0; i < 17; i++) {
  	  	tmp = strstr (cpu_info[i], ":");//忽略冒号:
		tmp += 2; 
               cpu_info[i] = tmp;	  
       }
       g_string_append (string, "CPU Model:");
       g_string_append (string, cpu_info[4]);
       g_string_append_c (string, '\n');
       g_string_append (string, "CPU cores\t:");
       g_string_append (string, cpu_info[12]);
       g_string_append_c (string, '\n');
       g_string_append (string, "CPU rate\t:");
       g_string_append (string, cpu_info[7]);
       g_string_append(string, "MHz");
       g_string_append_c (string, '\n');
       g_string_append (string, "Cache size\t:");
       g_string_append (string, cpu_info[8]);
       g_string_append_c (string, '\n');
}



void get_os_info (GString *string)//获得操作系统相关信息
{
    int fd,i,j;
    gchar buffer[128];
    gchar *tmp,*start,*stop;
    gchar *os_info[8];
    gchar *delim = " ";

    gchar buffer1[128];
    gchar *os_info1[3];
    gchar *delim1 = ".";

    fd = open ("/proc/version", O_RDONLY);//读取操作系统相关信息的文件
    read(fd, buffer, sizeof (buffer));

    start = buffer;
    stop = strstr (buffer, "#");//找到"#"在buffer中第一次出现的位置,返回该位置的指针
    stop--;
    stop--;
    os_info[0] = strtok (buffer, delim);
    for (i = 1; i < 8; i++)
    {
        os_info[i] = strtok (NULL, delim);//分割字符串

    }

    for(j=0;j<7;j++){
    	buffer1[j] = os_info[2][j];
    }
    os_info1[0] = strtok(buffer1,delim1);
    for(j = 1; j<3; j++)
    {
    	os_info1[j] = strtok(NULL,delim1);
    }

    //字符串串接显示相关信息
    g_string_append (string, "Os\t\t\t\t:");
    g_string_append (string, os_info[0]);
    g_string_append_c (string, '\n');
    g_string_append (string, "Release\t\t:");
    g_string_append (string, os_info[2]);
    /*
    g_string_append (string, os_info1[0]);
    g_string_append (string, ".");
    g_string_append (string, os_info1[1]);
    //g_string_append (string, ".");
    //g_string_append (string, os_info1[2]);
    */
    g_string_append_c (string, '\n');
    g_string_append (string, "Domain\t\t:");
    g_string_append (string, os_info[3]);
    g_string_append_c (string, '\n');
    g_string_append (string, "Gcc version\t:");
    g_string_append (string, os_info[6]);
    g_string_append_c (string, ' ');
    g_string_append (string, os_info[7]);
    g_string_append_c (string, '\n');
}


void draw_cpu_load_graph ()//CPU使用记录绘图
{
    if (cgraph == NULL)
        return;
    int i;
    gchar buffer[64];
    gint width,height,current,step;
    guint context_id;

    sprintf (buffer, "CPU using rate:%.2f%%",100 * rate);
    context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (status_bar),"cpu");

    gtk_statusbar_push (GTK_STATUSBAR (status_bar), context_id, buffer);

    //Clear PastGraph
    gdk_draw_rectangle (GDK_DRAWABLE (cgraph), window->style->white_gc, TRUE, 0, 0,
                        cpu_draw_area->allocation.width,
                        cpu_draw_area->allocation.height);
    width = cpu_draw_area->allocation.width;//Get Size
    height = cpu_draw_area->allocation.height;

    current = (int)(rate*(double)height);//Adjust Height
    cpu_graph[DENSITY-1] = height - current;  //Move Direction
    for ( i = 0 ; i < DENSITY - 1 ; i ++)
    {
        cpu_graph[i] = cpu_graph[i+1];
    }

    step = width/DENSITY;//stepsets
    GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(cgraph));
    GdkColor color;
    gdk_color_parse("#FFFF00", &color);
    gdk_gc_set_foreground(gc, &color);

    for (i = DENSITY - 1; i >= 1 ; i -- )
    {
        gdk_draw_line (cgraph, gc, i * step, cpu_graph[i],
                       (i - 1) * step, cpu_graph[i-1]  );
    }

    gtk_widget_queue_draw (cpu_draw_area);   //触发信号,刷新图片的整个区域
}

void draw_mem_load_graph ()
{
    if (mgraph == NULL) return;
    int fd,i;
    gint width,height,current,step;
    gint mem_total,mem_free;
    gchar buffer[540];
    gchar mem_tmp[19][20];
    gchar *start,*stop,*tmp;
    GString *string;

    fd = open ("/proc/meminfo", O_RDONLY);
    read (fd, buffer, sizeof (buffer));
    stop = buffer;
    close(fd);
    for (i = 0; i < 19; i++)
    {
        start = strstr (stop, ":");
        start++;
        stop = strstr (start, "kB");
        stop--;
        tmp = mem_tmp[i];
        while (start != stop) *tmp++ = *start++;
        *tmp = '\0';
    }
    mem_total = atoi (mem_tmp[0]);
    mem_free = atoi (mem_tmp[1]);


    string = g_string_new ("");//Refresh info_Num
    g_string_append (string, "Total Memory :");
    g_string_append (string, mem_tmp[0]);
    g_string_append (string, " kB");
    g_string_append (string, "   ");
    g_string_append (string, "Free Memory  :");
    g_string_append (string, mem_tmp[1]);
    g_string_append (string, " kB");
    g_string_append_c (string, '\n');
    gtk_label_set_text (GTK_LABEL (mem_label), string->str);
    g_string_free (string, TRUE);


    string = g_string_new ("");
    g_string_append (string, "Total Swap   :");
    g_string_append (string, mem_tmp[11]);
    g_string_append (string, " kB");
    g_string_append (string, "   ");
    g_string_append (string, "Free Swap    :");
    g_string_append (string, mem_tmp[12]);
    g_string_append (string, " kB");
    g_string_append_c (string, '\n');
    gtk_label_set_text (GTK_LABEL (swap_label), string->str);
    g_string_free (string, TRUE);


    //ClearGraph
    gdk_draw_rectangle (mgraph, window->style->white_gc, TRUE, 0, 0,
                        mem_draw_area->allocation.width,
                        mem_draw_area->allocation.height);

    width = mem_draw_area->allocation.width;//GetSize
    height = mem_draw_area->allocation.height;
    current = ((float)(mem_total-mem_free)/(float)mem_total) * height;//Adjust Size
    mem_graph[DENSITY-1] = height - current; //Move Dir
    for ( i = 0 ; i < DENSITY - 1 ; i ++)
    {
        mem_graph[i] = mem_graph[i+1];
    }
    step = width / DENSITY;

    GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(mgraph));
    GdkColor color;
    gdk_color_parse("#0000EE", &color);
    gdk_gc_set_foreground(gc, &color);

    for (i = DENSITY - 1; i >= 1 ; i -- )
    {
        gdk_draw_line (mgraph, gc, i * step,
                       mem_graph[i], (i - 1) * step, mem_graph[i-1]);
    }
    gtk_widget_queue_draw ( mem_draw_area);
}

void prefresh_button_clicked (gpointer data)
{
    gtk_list_store_clear (process_store);
    get_process_info (process_store);
}

void pdelete_button_clicked (gpointer data)//kill按钮被点击,停止一个进程的运行
{
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *pid;
    pid_t pid_num;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptree_view));//获得当前选择的项
    if (gtk_tree_selection_get_selected(selection, &model, &iter))
    {
        gtk_tree_model_get (model, &iter, PID_COLUMN, &pid, -1);//在书的相应列中获得该进程的PID
        pid_num = atoi (pid);//字符串转换成长整数

        if(kill (pid_num, SIGTERM) == -1 )//根据PID结束该进程
        {
            gchar *title = "ERROR";
            gchar *content = "Termination Failed,Check UID";
            show_dialog (title, content);//结束进程失败信息输出
        }
        gtk_list_store_clear (process_store);//刷新进程信息
        get_process_info (process_store);
    }
}


gboolean cpu_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    if (cgraph)
    {
        g_object_unref (cgraph);
    }

    //Pixmap Create
    cgraph = gdk_pixmap_new (widget->window,
                             widget->allocation.width, widget->allocation.height,-1);

    //RePaint in Pixmap
    gdk_draw_rectangle (cgraph, widget->style->white_gc, TRUE, 0, 0,
                        widget->allocation.width, widget->allocation.height);
    return TRUE;
}

gboolean cpu_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    gdk_draw_drawable (widget->window,
                       widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                       cgraph,
                       event->area.x, event->area.y,
                       event->area.x, event->area.y,
                       event->area.width, event->area.height);
    return TRUE;
}


//Graph Refreshing
gboolean mem_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    if (mgraph)
    {
        g_object_unref (mgraph);
    }

    mgraph = gdk_pixmap_new (widget->window,
                             widget->allocation.width, widget->allocation.height,-1);

    gdk_draw_rectangle (mgraph, widget->style->white_gc, TRUE, 0, 0,
                        widget->allocation.width, widget->allocation.height);
    return TRUE;
}

gboolean mem_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    gdk_draw_drawable (widget->window,
                       widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                       mgraph,
                       event->area.x, event->area.y,
                       event->area.x, event->area.y,
                       event->area.width, event->area.height);
    return TRUE;
}



void show_dialog (gchar *title, gchar *content)//显示一个对话框,标题title,内容为content
{
    GtkWidget *dialog;
    GtkWidget *label;
    dialog = gtk_dialog_new_with_buttons (title,
                                          GTK_WINDOW (window),
                                          GTK_DIALOG_DESTROY_WITH_PARENT,
                                          GTK_STOCK_CLOSE,
                                          GTK_RESPONSE_NONE,
                                          NULL);//创建一个对话框控件
    gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);//不可改变大小
    g_signal_connect_swapped (dialog,
                              "response",
                              G_CALLBACK (gtk_widget_destroy),
                              dialog);//事件回调函数为退出

    label = gtk_label_new (content);//创建一个标签,显示content的内容
    gtk_widget_show (label);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox),
                       label);
    gtk_widget_show (dialog);
}
