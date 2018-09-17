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

//磁盘操作接口
#define SYS_SIZE (100 * 1024 * 1024) //系统大小
#define BLOCK_SIZE (1024)            //盘块大小
#define BLOCK_COUNT (100 * 1024)     //系统盘块数目
#define DIR_MAX_SIZE 15 //目录表项最大值

//目录项 64B
typedef struct dirunit
{
    char fileName[59]; //文件名
    char type;         //文件类型,0目录， 1文件
    int startBlock;    //起始盘块
} DIR;

//目录表
//一个目录表占用一个盘块，允许最多文件数为15
typedef struct DIR_TABLE
{
    int dirUnitAmount;      //目录项数目
    DIR dirs[DIR_MAX_SIZE]; //目录项列表
} DIR_TABLE;

//INODE
typedef struct inode
{
    int blockNum; //文件数据起始盘块号
    int fileSize; //文件大小，盘块为单位
    int dataSize; //已写入的内容大小，字节为单位
    int readptr;  //读指针，字节为单位
    int link;     //文件链接数
} INODE;

//初始化系统
void initSystem(void);
//磁盘分配
int getBlock(int blockSize);
//获得盘块的物理地址
char *getBlockAddr(int blockNum);
//获得物理地址的盘块号
int getAddrBlock(char *addr);
//释放盘块、
int releaseBlock(int blockNum, int blockSize);
//退出系统
void exitSystem();

#endif // __DISK_h__
