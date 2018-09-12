#ifndef FILEOPERATE_H_INCLUDED
#define FILEOPERATE_H_INCLUDED

//文件操作
#include"DiskOperate.h"

#define dirTable_max_size 15    //目录表项最大值

//目录项 64B
struct dirUnit{
    char fileName[59];  //文件名
    char type;  //文件类型,0目录， 1文件
    int startBlock; //起始盘块
};
//目录表
    //一个目录表占用一个盘块，允许最多文件数为15
struct dirTable {
    int dirUnitAmount;//目录项数目
    dirUnit dirs[dirTable_max_size];//目录项列表
};

//FCB
struct FCB {
    int blockNum;   //文件数据起始盘块号
    int fileSize;   //文件大小，盘块为单位
    int dataSize;   //已写入的内容大小，字节为单位
    int readptr;    //读指针，字节为单位
    int link;   //文件链接数
};

//文件系统操作
//初始化根目录
void initRootDir();
//展示绝对路径
char* getPath();
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
int read(char fileName[], int length);
//重新读文件
int reread(char fileName[], int length);
//执行读操作
int doRead(FCB* myFCB, int length);
//写文件，从末尾写入
int write(char fileName[], char content[]);
//重新写覆盖
int rewrite(char fileName[], char content[]);
//执行写操作
int doWrite(FCB* myFCB, char content[]);

//释放文件内存
int releaseFile(int FCBBlock);
//添加目录项
int addDirUnit(dirTable* currentDirTable, char fileName[], int type, int FCBBlockNum);
//创建FCB
int creatFCB(int fcbBlockNum, int fileBlockNum, int fileSize);
//指定目录删除
int deleteFileInTable(dirTable* myDirTable, int unitIndex);
//删除目录项
int deleteDirUnit(dirTable* currentDirTable, int unitIndex);
//从目录中查找目录项目
int findUnitInTable(dirTable* currentDirTable, char unitName[]);


#endif // FILEOPERATE_H_INCLUDED