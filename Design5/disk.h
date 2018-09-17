/***********************************************************************
 * @file disk.h
     DISK
 * @brief   header file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/
#ifndef __DISK_h__
#define __DISK_h__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define INODE_NUM 2048         //i节点数目
#define BLOCK_NUM (100 * 1024) //磁盘块的数目
#define BLOCK_SIZE 1024        //磁盘块大小为1K
#define BlkPerNode 1024        //每个文件包含的最大的磁盘块数目
#define DISK "disk.img"
#define BUFF ".buff.txt"                                     //读写文件时的缓冲文件
#define SUPER_BEGIN 0                                          //超级块的起始地址
#define INODE_BEGIN sizeof(SUPER_BLOCK)                        //i节点区启示地址
#define BLOCK_BEGIN (INODE_BEGIN + INODE_NUM * sizeof(INODE))     //数据区起始地址
#define MaxDirNum (BlkPerNode * (BLOCK_SIZE / sizeof(DIR))) //每个目录最大的文件数
#define DirPerBlk (BLOCK_SIZE / sizeof(DIR))                //每个磁盘块包含的最大目录项



typedef struct superblk
{
    char inode_map[INODE_NUM]; //i节点位图
    char blk_map[BLOCK_NUM];   //磁盘块位图
    int inode_used;            //已被使用的i节点数目
    int blk_used;              //已被使用的磁盘块数目
} SUPER_BLOCK;

typedef struct inode
{
    int blk_identifier[BlkPerNode]; //占用的磁盘块编号
    int blk_num;                    //占用的磁盘块数目
    int file_size;                  //文件的大小
    int type;                       //文件的类型
} INODE;

typedef struct dir
{
    char name[30];   //目录名
    short inode_num; //目录对应的inode
} DIR;

extern DIR dir_table[MaxDirNum]; //将当前目录文件的内容都载入内存
extern int dir_num;              //相应编号的目录项数
extern int inode_num;            //当前目录的inode编号
extern INODE curr_inode;         //当前目录的inode结构
extern SUPER_BLOCK super_blk;    //文件系统的超级块
extern FILE *Disk;
extern char path[40];

int init_fs(void);   //初始化文件系统
int close_fs(void);  //关闭文件系统
int format_fs(void); //格式化文件系统

int free_blk(int); //释放相应的磁盘块
int get_blk(void); //获取磁盘块

#endif // __DISK_h__
