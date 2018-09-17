/***********************************************************************
 * @file disk.c
     DISK
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "disk.h"
#include "file.h"

DIR dir_table[MaxDirNum]; //将当前目录文件的内容都载入内存
int dir_num;              //相应编号的目录项数
int inode_num;            //当前目录的inode编号
INODE curr_inode;         //当前目录的inode结构
SUPER_BLOCK super_blk;    //文件系统的超级块
FILE *Disk;
char path[40] = "/";

int init_fs(void)
{
    fseek(Disk, SUPER_BEGIN, SEEK_SET);
    fread(&super_blk, sizeof(SUPER_BLOCK), 1, Disk); //读取超级块

    inode_num = 0;
    if (!open_dir(inode_num))
    {
        printf("CANT'T OPEN ROOT DIRECTORY\n");
        return 0;
    }

    return 1;
}

int close_fs(void)
{
    fseek(Disk, SUPER_BEGIN, SEEK_SET);
    fwrite(&super_blk, sizeof(SUPER_BLOCK), 1, Disk);

    close_dir(inode_num);
    return 1;
}

int format_fs(void)
{
    /*格式化inode_map,保留根目录*/
    memset(super_blk.inode_map, 0, sizeof(super_blk.inode_map));
    super_blk.inode_map[0] = 1;
    super_blk.inode_used = 1;
    /*格式化blk_map,保留第一个磁盘块给根目录*/
    memset(super_blk.blk_map, 0, sizeof(super_blk.blk_map));
    super_blk.blk_map[0] = 1;
    super_blk.blk_used = 1;

    inode_num = 0; //将当前目录改为根目录

    /*读取根目录的i节点*/
    fseek(Disk, INODE_BEGIN, SEEK_SET);
    fread(&curr_inode, sizeof(INODE), 1, Disk);
    //	printf("%d\n",curr_inode.file_size/sizeof(DIR));

    curr_inode.file_size = 2 * sizeof(DIR);
    curr_inode.blk_num = 1;
    curr_inode.blk_identifier[0] = 0; //第零块磁盘一定是根目录的

    /*仅.和..目录项有效*/
    dir_num = 2;

    strcpy(dir_table[0].name, ".");
    dir_table[0].inode_num = 0;
    strcpy(dir_table[1].name, "..");
    dir_table[1].inode_num = 0;

    strcpy(path, "monster: root");

    return 1;
}


/*申请未被使用的磁盘块*/
int get_blk()
{
    int i;
    super_blk.blk_used++;
    for (i = 0; i < BLOCK_NUM; ++i)
    { //找到未被使用的块
        if (!super_blk.blk_map[i])
        {
            super_blk.blk_map[i] = 1;
            return i;
        }
    }

    return -1; //没有多余的磁盘块
}

/*释放磁盘块*/
int free_blk(int blk_pos)
{
    super_blk.blk_used--;
    super_blk.blk_map[blk_pos] = 0;
    return 0;
}
