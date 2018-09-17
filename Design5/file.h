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

#define F_DIR  0
#define F_FILE 1

//文件系统操作
//初始化根目录
void initRootDir();
//展示绝对路径
char *getPath();
//展示当前目录文件
void showDir();
//创建目录
int creatDir(char dirName[]);
//切换目录
int changeDir(char dirName[]);
//删除目录
int deleteDir(char fileName[]);
//修改文件名或者目录名
int changeName(char oldName[], char newName[]);

//创建文件
int creatFile(char fileName[], int fileSize);
//删除文件
int deleteFile(char fileName[]);

//读文件
int file_read(char fileName[], int length);
//重新读文件
int reread(char fileName[], int length);
//执行读操作
int doRead(INODE *myFCB, int length);
//写文件，从末尾写入
int file_write(char fileName[], char content[]);
//重新写覆盖
int rewrite(char fileName[], char content[]);
//执行写操作
int doWrite(INODE *myFCB, char content[]);

//释放文件内存
int releaseFile(int FCBBlock);
//添加目录项
int addDirUnit(DIR_TABLE *currentDirTable, char fileName[], int type, int FCBBlockNum);
//创建FCB
int creatFCB(int fcbBlockNum, int fileBlockNum, int fileSize);
//指定目录删除
int deleteFileInTable(DIR_TABLE *myDirTable, int unitIndex);
//删除目录项
int deleteDirUnit(DIR_TABLE *currentDirTable, int unitIndex);
//从目录中查找目录项目
int findUnitInTable(DIR_TABLE *currentDirTable, char unitName[]);

#endif // __FILE_h__
