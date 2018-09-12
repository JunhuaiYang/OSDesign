/*
*磁盘空间采用连续分配的方式，以1kB作为盘快为分配单位，采用位示图记录盘块分配情况（位于盘块0）
*系统共有100k个盘块，前100个盘块用于存放位图表（char[]）
*文件目录采用树状目录，系统创始化一张根目录表（用户初始位置）；占用一个盘块
*目录项：文件名，FCB盘块号，文件类型
*文件控制块FCB：文件数据起始盘块号，文件大小，链接数,读指针，数据大小；占用一个盘块
*文件数据和FCB分开存放
*/

#include"FileOperate.h"
#include"DiskOperate.h"
//#include <iostream>
#include <stdio.h>
#include <string.h>
//using namespace std;

//输出帮助
void help();

int main()
{
    //---进程同步读写
    //int readCount = 0; //当前阅读人数
    //semaphore rmutex = 1; //readCount互斥访问
    //semaphore mutex = 1;  //进程对文件的互斥访问

    //初始化系统
    initSystem();
    //创建根目录
    initRootDir();

    help();
    char command[10];
    printf("littleqi:\\>");
    scanf("%s", command);
    while(strcmp(command, "quit") != 0)
    {
        if(strcmp(command, "ls") == 0)//列出目录下所有文件
        {
            showDir();
            //printf("ls\n");
        }else if(strcmp(command, "help") == 0)//帮助
        {
            help();
        }else if(strcmp(command, "cd") == 0)//切换目录
        {
            char name[59];
            scanf("%s", name);
            changeDir(name);
        }else if(strcmp(command, "pwd") == 0)//显示目录
        {
            printf("%s\n", getPath());
        }else if(strcmp(command, "mkdir") == 0)//创建目录
        {
            char name[59];
            scanf("%s", name);
            creatDir(name);
        }else if(strcmp(command, "rmdir") == 0)//删除目录
        {
            char name[59];
            scanf("%s", name);
            deleteDir(name);
        }else if(strcmp(command, "mv") == 0)//修改文件或者目录名
        {
            char oldname[59];
            scanf("%s", oldname);
            char newname[59];
            scanf("%s", newname);
            changeName(oldname, newname);
        }else if(strcmp(command, "touch") == 0)//创建文件
        {
            char name[59];
            scanf("%s", name);
            int fileSize;
            scanf("%d", &fileSize);
            creatFile(name, fileSize);
        }else if(strcmp(command, "rm") == 0)//删除文件
        {
            char name[59];
            scanf("%s", name);
            deleteFile(name);
        }else if(strcmp(command, "read") == 0)//读取文件
        {
            char name[59];
            scanf("%s", name);
            int length;
            scanf("%d", &length);
            /*
            wait(rmutex)
            if(readCount==0)
                wait(mutex);
            readCount++;
            signal(rmutex);
            */
            read(name, length);
            /*
            wait(rmutex);
            readCount--;
            if(readCount == 0)
                signal(mutex);
            signal(rmutex);
            */
        }else if(strcmp(command, "reread") == 0)//重设读指针为起点
        {
            char name[59];
            scanf("%s", name);
            int length;
            scanf("%d", &length);
            /*
            wait(rmutex)
            if(readCount==0)
                wait(mutex);
            readCount++;
            signal(rmutex);
            */
            reread(name, length);
            /*
            wait(rmutex);
            readCount--;
            if(readCount == 0)
                signal(mutex);
            signal(rmutex);
            */
        }else if(strcmp(command, "write") == 0)//写文件,只支持从末尾写入
        {
            char name[59];
            scanf("%s", name);
            char content[1024];
            scanf("%s", content);
            //wait(mutex);
            write(name, content);
            //signal(mutex);
        }else if(strcmp(command, "rewrite") == 0)//重新写文件
        {
            char name[59];
            scanf("%s", name);
            char content[1024];
            scanf("%s", content);
            //wait(mutex);
            rewrite(name, content);
            //signal(mutex);
        }else
        {
            printf("command error\n");
        }

        printf("littleqi:%s>", getPath());
        scanf("%s", command);
    }
    exitSystem();

    return 0;
}
void help()
{
    printf("目录操作命令：\nls\npwd\ncd dirName\nmkdir dirName\nrmdir dirName\nmv fileName newName\n");
    printf("文件操作命令：\ntouch fileName size(文件大小) ep:touch a.txt 10\nrm fileName\n");
    printf("read fileName size(读取字节长度) 从上一次开始读ep:read a.txt 10\nreread fileName size 重头开始读\n");
    printf("write fileName content(写入内容) 从末尾写入ep:write a.txt helloworld\nrewrite fileName content 重头开始写\n");
    printf("\n");
}