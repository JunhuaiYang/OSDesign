/***********************************************************************
 * @file file.h
     FILE
 * @brief   header file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/
#ifndef __FILE_h__
#define __FILE_h__

#include "disk.h"

#define D_DIR 0
#define D_FILE 1

int open_dir(int);               //打开相应inode对应的目录
int close_dir(int);              //保存相应inode的目录
int show_dir(int);               //显示目录
int make_file(int, char *, int); //创建新的目录或文件
int del_file(int, char *, int);  //删除子目录
int enter_dir(int, char *);      //进入子目录

int file_write(char *); //写文件
int file_read(char *);  //读文件

int adjust_dir(char *); //删除子目录后，调整原目录，使中间无空隙

int check_name(int, char *); //检查重命名,返回-1表示名字不存在，否则返回相应inode
int type_check(char *);      //确定文件的类型

int free_inode(int);          //释放相应的inode
int apply_inode();            //申请inode,返还相应的inode号，返还-1则INODE用完
int init_dir_inode(int, int); //初始化新建目录的inode
int init_file_inode(int);     //初始化新建文件的inode

void change_path(char *);

#endif // __FILE_h__
