/***********************************************************************
 * @file main.c
     MAIN
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "disk.h"
#include "file.h"

//输出帮助
void help();

int main()
{
    //初始化系统
    initSystem();
    //创建根目录
    initRootDir();

    printf("\nWelcome to Simple File System! \n\n");
    char command[20];
    printf("junhuai@yjh:\e[1;32m%s\e[0m$ ", getPath());
    // 判断命令是否操作
    scanf("%s", command);
    while (strcmp(command, "quit") && strcmp(command, "exit"))
    {
        if (strcmp(command, "ls") == 0) //列出目录下所有文件
        {
            showDir();
        }
        else if (strcmp(command, "cd") == 0) //切换目录
        {
            char name[59];
            scanf("%s", name);
            ChangeDir(name);
        }
        else if (strcmp(command, "mkdir") == 0) //创建目录
        {
            char name[59];
            scanf("%s", name);
            creatDir(name);
        }
        else if (strcmp(command, "pwd") == 0) //显示目录
        {
            printf("%s\n", getPath());
        }
        else if (strcmp(command, "rmdir") == 0) //删除目录
        {
            char name[59];
            scanf("%s", name);
            deleteDir(name);
        }
        else if (strcmp(command, "mv") == 0) //修改文件或者目录名
        {
            char oldname[59];
            scanf("%s", oldname);
            char newname[59];
            scanf("%s", newname);
            changeName(oldname, newname);
        }
        else if (strcmp(command, "touch") == 0) //创建文件
        {
            char name[59];
            scanf("%s", name);
            int fileSize;
            scanf("%d", &fileSize);
            creatFile(name, fileSize);
        }
        else if (strcmp(command, "rm") == 0) //删除文件
        {
            char name[59];
            scanf("%s", name);
            deleteFile(name);
        }
        else if (strcmp(command, "read") == 0) //读取文件
        {
            char name[59];
            scanf("%s", name);
            int length;
            scanf("%d", &length);
            file_read(name, length);
        }
        else if (strcmp(command, "reread") == 0) //重设读指针为起点
        {
            char name[59];
            scanf("%s", name);
            int length;
            scanf("%d", &length);
            reread(name, length);
        }
        else if (strcmp(command, "write") == 0) //写文件,只支持从末尾写入
        {
            char name[59];
            scanf("%s", name);
            char content[1024];
            scanf("%s", content);
            file_write(name, content);
        }
        else if (strcmp(command, "rewrite") == 0) //重新写文件
        {
            char name[59];
            scanf("%s", name);
            char content[1024];
            scanf("%s", content);
            rewrite(name, content);
        }
        else if (strcmp(command, "help") == 0) //帮助
        {
            help();
        }
        else
        {
            printf("command not found! \n");
        }

        // 颜色显示  用绿色显示当前目录
        printf("junhuai@yjh:\e[1;32m%s\e[0m$ ", getPath());
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