/***********************************************************************
 * @file file.c
     FILE
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "file.h"

int open_dir(int inode)
{
    int i;
    int pos = 0;
    int left;
    fseek(Disk, INODE_BEGIN + sizeof(INODE) * inode, SEEK_SET);

    /*读出相应的i节点*/
    fread(&curr_inode, sizeof(INODE), 1, Disk);

    for (i = 0; i < curr_inode.blk_num - 1; ++i)
    {
        fseek(Disk, BLOCK_BEGIN + BLOCK_SIZE * curr_inode.blk_identifier[i], SEEK_SET);
        fread(dir_table + pos, sizeof(DIR), DirPerBlk, Disk);
        pos += DirPerBlk;
    }

    /*left为最后一个磁盘块内的目录项数*/
    left = curr_inode.file_size / sizeof(DIR) - DirPerBlk * (curr_inode.blk_num - 1);
    fseek(Disk, BLOCK_BEGIN + BLOCK_SIZE * curr_inode.blk_identifier[i], SEEK_SET);
    fread(dir_table + pos, sizeof(DIR), left, Disk);
    pos += left;

    dir_num = pos;

    return 1;
}

int close_dir(int inode)
{
    int i, pos = 0, left;

    /*数据写回磁盘块*/
    for (i = 0; i < curr_inode.blk_num - 1; ++i)
    {
        fseek(Disk, BLOCK_BEGIN + BLOCK_SIZE * curr_inode.blk_identifier[i], SEEK_SET);
        fwrite(dir_table + pos, sizeof(DIR), DirPerBlk, Disk);
        pos += DirPerBlk;
    }

    left = dir_num - pos;
    //	printf("left:%d",left);
    fseek(Disk, BLOCK_BEGIN + BLOCK_SIZE * curr_inode.blk_identifier[i], SEEK_SET);
    fwrite(dir_table + pos, sizeof(DIR), left, Disk);

    /*inode写回*/
    curr_inode.file_size = dir_num * sizeof(DIR);
    fseek(Disk, INODE_BEGIN + inode * sizeof(INODE), SEEK_SET);
    fwrite(&curr_inode, sizeof(curr_inode), 1, Disk);

    return 1;
}

/*创建新的目录项*/
int make_file(int inode, char *name, int type)
{
    int new_node;
    int blk_need = 1; //本目录需要增加磁盘块则blk_need=2
    int t;

    if (dir_num > MaxDirNum)
    { //超过了目录文件能包含的最大目录项
        printf("mkdir: cannot create directory '%s' :D_DIR full\n", name);
        return 0;
    }

    if (check_name(inode, name) != -1)
    { //防止重命名
        printf("mkdir: cannnot create file '%s' :D_FILE exist\n", name);
        return 0;
    }

    if (dir_num / DirPerBlk != (dir_num + 1) / DirPerBlk)
    { //本目录也要增加磁盘块
        blk_need = 2;
    }

    //	printf("blk_used:%d\n",super_blk.blk_used);
    if (super_blk.blk_used + blk_need > BLOCK_NUM)
    {

        printf("mkdir: cannot create file '%s' :Block used up\n", name);
        return 0;
    }

    if (blk_need == 2)
    { //本目录需要增加磁盘块
        t = curr_inode.blk_num++;
        curr_inode.blk_identifier[t] = get_blk();
    }

    /*申请inode*/
    new_node = apply_inode();

    if (new_node == -1)
    {
        printf("mkdir: cannot create file '%s' :INODE used up\n", name);
        return 0;
    }

    if (type == D_DIR)
    {
        /*初始化新建目录的inode*/
        init_dir_inode(new_node, inode);
    }
    else if (type == D_FILE)
    {
        /*初始化新建文件的inode*/
        init_file_inode(new_node);
    }

    strcpy(dir_table[dir_num].name, name);
    dir_table[dir_num++].inode_num = new_node;
}

/*显示目录内容*/
int show_dir(int inode)
{
    int i, color = 32;
    for (i = 0; i < dir_num; ++i)
    {
        if (type_check(dir_table[i].name) == D_DIR)
        {
            /*目录显示绿色*/
            printf("\033[1;%dm%s\t\033[0m", color, dir_table[i].name);
        }
        else
        {
            printf("%s\t", dir_table[i].name);
        }
        if (!((i + 1) % 4))
            printf("\n"); //4个一行
    }
    printf("\n");

    return 1;
}

/*申请inode*/
int apply_inode()
{
    int i;

    if (super_blk.inode_used >= INODE_NUM)
    {
        return -1; //inode节点用完
    }

    super_blk.inode_used++;

    for (i = 0; i < INODE_NUM; ++i)
    {
        if (!super_blk.inode_map[i])
        { //找到一个空的i节点
            super_blk.inode_map[i] = 1;
            return i;
        }
    }
}

int free_inode(int inode)
{
    INODE temp;
    int i;
    fseek(Disk, INODE_BEGIN + sizeof(INODE) * inode, SEEK_SET);
    fread(&temp, sizeof(INODE), 1, Disk);

    for (i = 0; i < temp.blk_num; ++i)
    {
        free_blk(temp.blk_identifier[i]);
    }

    super_blk.inode_map[inode] = 0;
    super_blk.inode_used--;

    return 1;
}

/*进入子目录*/
int enter_dir(int inode, char *name)
{
    int child;
    child = check_name(inode, name);

    if (child == -1)
    { //该子目录不存在
        printf("cd: %s: No such file or directory\n", name);
        return 0;
    }

    /*关闭当前目录,进入下一级目录*/
    close_dir(inode);
    inode_num = child;
    open_dir(child);

    return 1;
}

/*递归删除文件夹*/
int del_file(int inode, char *name, int deepth)
{
    int child, i, t;
    INODE temp;

    if (!strcmp(name, ".") || !strcmp(name, ".."))
    {
        /*不允许删除.和..*/
        printf("rmdir: failed to remove '%s': Invalid argument\n", name);
        return 0;
    }

    child = check_name(inode, name);

    if (child == -1)
    { //子目录不存在
        printf("rmdir: failed to remove '%s': No such file or directory\n", name);
    }

    /*读取当前子目录的Inode结构*/
    fseek(Disk, INODE_BEGIN + sizeof(INODE) * child, SEEK_SET);
    fread(&temp, sizeof(INODE), 1, Disk);

    if (temp.type == D_FILE)
    {
        /*如果是文件则释放相应Inode即可*/
        free_inode(child);
        /*若是最上层文件，需调整目录*/
        if (deepth == 0)
        {
            adjust_dir(name);
        }
        return 1;
    }
    else
    {
        /*否则进入子目录*/
        enter_dir(inode, name);
    }

    for (i = 2; i < dir_num; ++i)
    {
        del_file(child, dir_table[i].name, deepth + 1);
    }

    enter_dir(child, ".."); //返回上层目录
    free_inode(child);

    if (deepth == 0)
    {
        /*删除自身在目录中的内容*/
        if (dir_num / DirPerBlk != (dir_num - 1) / DirPerBlk)
        {
            /*有磁盘块可以释放*/
            curr_inode.blk_num--;
            t = curr_inode.blk_identifier[curr_inode.blk_num];
            free_blk(t); //释放相应的磁盘块
        }
        adjust_dir(name); //因为可能在非末尾处删除，因此要移动dir_table的内容
    }                     /*非初始目录直接释放Inode*/

    return 1;
}

int adjust_dir(char *name)
{
    int pos;
    for (pos = 0; pos < dir_num; ++pos)
    {
        /*先找到被删除的目录的位置*/
        if (strcmp(dir_table[pos].name, name) == 0)
            break;
    }
    for (pos++; pos < dir_num; ++pos)
    {
        /*pos之后的元素都往前移动一位*/
        dir_table[pos - 1] = dir_table[pos];
    }

    dir_num--;
    return 1;
}

/*初始化新建目录的inode*/
int init_dir_inode(int child, int father)
{
    INODE temp;
    DIR dot[2];
    int blk_pos;

    fseek(Disk, INODE_BEGIN + sizeof(INODE) * child, SEEK_SET);
    fread(&temp, sizeof(INODE), 1, Disk);

    blk_pos = get_blk(); //获取新磁盘块的编号

    temp.blk_num = 1;
    temp.blk_identifier[0] = blk_pos;
    temp.type = D_DIR;
    temp.file_size = 2 * sizeof(DIR);
    /*将初始化完毕的Inode结构写回*/
    fseek(Disk, INODE_BEGIN + sizeof(INODE) * child, SEEK_SET);
    fwrite(&temp, sizeof(INODE), 1, Disk);

    strcpy(dot[0].name, "."); //指向目录本身
    dot[0].inode_num = child;

    strcpy(dot[1].name, "..");
    dot[1].inode_num = father;

    /*将新目录的数据写进数据块*/
    fseek(Disk, BLOCK_BEGIN + BLOCK_SIZE * blk_pos, SEEK_SET);
    fwrite(dot, sizeof(DIR), 2, Disk);

    return 1;
}

/*初始化新建文件的indoe*/
int init_file_inode(int inode)
{
    INODE temp;
    /*读取相应的Inode*/
    fseek(Disk, INODE_BEGIN + sizeof(INODE) * inode, SEEK_SET);
    fread(&temp, sizeof(INODE), 1, Disk);

    temp.blk_num = 0;
    temp.type = D_FILE;
    temp.file_size = 0;
    /*将已经初始化的Inode写回*/
    fseek(Disk, INODE_BEGIN + sizeof(INODE) * inode, SEEK_SET);
    fwrite(&temp, sizeof(INODE), 1, Disk);

    return 1;
}



/*检查重命名*/
int check_name(int inode, char *name)
{
    int i;
    for (i = 0; i < dir_num; ++i)
    {
        /*存在重命名*/
        if (strcmp(name, dir_table[i].name) == 0)
        {
            return dir_table[i].inode_num;
        }
    }

    return -1;
}

void change_path(char *name)
{
    int pos;
    if (strcmp(name, ".") == 0)
    { //进入本目录则路径不变
        return;
    }
    else if (strcmp(name, "..") == 0)
    { //进入上层目录，将最后一个'/'后的内容去掉
        pos = strlen(path) - 1;
        for (; pos >= 0; --pos)
        {
            if (path[pos] == '/')
            {
                path[pos] = '\0';
                break;
            }
        }
    }
    else
    { //否则在路径末尾添加子目录
        strcat(path, "/");
        strcat(path, name);
    }

    return;
}

int type_check(char *name)
{
    int i, inode;
    INODE temp;
    for (i = 0; i < dir_num; ++i)
    {
        if (strcmp(name, dir_table[i].name) == 0)
        {
            inode = dir_table[i].inode_num;
            fseek(Disk, INODE_BEGIN + sizeof(INODE) * inode, SEEK_SET);
            fread(&temp, sizeof(INODE), 1, Disk);
            return temp.type;
        }
    }
    return -1; //该文件或目录不存在
}

/*读文件函数*/
int file_read(char *name)
{
    int inode, i, blk_num;
    INODE temp;
    FILE *fp = fopen(BUFF, "w+");
    char buff[BLOCK_SIZE];
    //printf("read\n");

    inode = check_name(inode_num, name);

    fseek(Disk, INODE_BEGIN + sizeof(INODE) * inode, SEEK_SET);
    fread(&temp, sizeof(temp), 1, Disk);

    if (temp.blk_num == 0)
    { //如果源文件没有内容,则直接退出
        fclose(fp);
        return 1;
    }
    printf("read\n");
    for (i = 0; i < temp.blk_num - 1; ++i)
    {
        blk_num = temp.blk_identifier[i];
        /*读出文件包含的磁盘块*/
        fseek(Disk, BLOCK_BEGIN + BLOCK_SIZE * blk_num, SEEK_SET);
        fread(buff, sizeof(char), BLOCK_SIZE, Disk);
        /*写入BUFF*/
        fwrite(buff, sizeof(char), BLOCK_SIZE, fp);
        free_blk(blk_num); //直接将该磁盘块释放
        temp.file_size -= BLOCK_SIZE;
    }

    /*最后一块磁盘块可能未满*/
    blk_num = temp.blk_identifier[i];
    fseek(Disk, BLOCK_BEGIN + BLOCK_SIZE * blk_num, SEEK_SET);
    fread(buff, sizeof(char), temp.file_size, Disk);
    fwrite(buff, sizeof(char), temp.file_size, fp);
    free_blk(blk_num);
    /*修改inode信息*/
    temp.file_size = 0;
    temp.blk_num = 0;

    /*将修改后的Inode写回*/
    fseek(Disk, INODE_BEGIN + sizeof(INODE) * inode, SEEK_SET);
    fwrite(&temp, sizeof(INODE), 1, Disk);

    fclose(fp);
    return 1;
}

/*写文件函数*/
int file_write(char *name)
{
    int inode, i;
    int num, blk_num;
    FILE *fp = fopen(BUFF, "r");
    INODE temp;
    char buff[BLOCK_SIZE];

    inode = check_name(inode_num, name);

    fseek(Disk, INODE_BEGIN + sizeof(INODE) * inode, SEEK_SET);
    fread(&temp, sizeof(INODE), 1, Disk);

    while (num = fread(buff, sizeof(char), BLOCK_SIZE, fp))
    {
        printf("num:%d\n", num);
        if ((blk_num = get_blk()) == -1)
        {
            printf("error:	block has been used up\n");
            break;
        }
        /*改变Inode结构的相应状态*/
        temp.blk_identifier[temp.blk_num++] = blk_num;
        temp.file_size += num;

        /*将数据写回磁盘块*/
        fseek(Disk, BLOCK_BEGIN + BLOCK_SIZE * blk_num, SEEK_SET);
        fwrite(buff, sizeof(char), num, Disk);
    }

    /*将修改后的Inode写回*/
    fseek(Disk, INODE_BEGIN + sizeof(INODE) * inode, SEEK_SET);
    fwrite(&temp, sizeof(INODE), 1, Disk);

    fclose(fp);
    return 1;
}