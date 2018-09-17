/***********************************************************************
 * @file main.c
     MAIN
 * @brief   source file
 *
 * @Copyright (C)  2018  YangJunhuai. all right reserved
***********************************************************************/

#include "disk.h"
#include "file.h"


int main()
{
    char command[30], name[30];
    char *arg[] = {"vim", BUFF, NULL};

    Disk = fopen(DISK, "at+");
    init_fs();

        printf("\nWelcome to Simple File System! \n\n");


    while (1)
    {
        printf("junhuai@yjh:\e[1;32m%s\e[0m$ ", path);
        scanf("%s", command);

        if (!strcmp(command, "quit") || !strcmp(command, "exit"))
        {
            break;
        }
        else if (!strcmp(command, "fmt"))
        {
            format_fs();
        }
        else if (!strcmp(command, "mkdir"))
        {
            scanf("%s", name);
            make_file(inode_num, name, D_DIR);
        }
        else if (!strcmp(command, "rmdir"))
        {
            scanf("%s", name);
            if (type_check(name) != D_DIR)
            {
                printf("rmdir: failed to remove '%s': Not a directory\n", name);
                break;
            }
            del_file(inode_num, name, 0);
        }
        else if (!strcmp(command, "cd"))
        {
            scanf("%s", name);
            if (type_check(name) != D_DIR)
            {
                printf("cd: %s: Not a directory\n", name);
                break;
            }
            if (enter_dir(inode_num, name))
            {
                change_path(name); //改变路径前缀
            }
        }
        else if (!strcmp(command, "ls"))
        {
            show_dir(inode_num);
        }
        else if (!strcmp(command, "mk"))
        {
            scanf("%s", name);
            make_file(inode_num, name, D_FILE);
        }
        else if (!strcmp(command, "rm"))
        {
            scanf("%s", name);
            if (type_check(name) != D_FILE)
            {
                printf("rm: cannot remove '%s': Not a file\n", name);
                break;
            }
            del_file(inode_num, name, 0);
        }
        else if (!strcmp(command, "vim"))
        {
            scanf("%s", name);
            if (type_check(name) != D_FILE)
            {
                printf("vim: cannot edit '%s': Not a file\n", name);
                break;
            }

            file_read(name); //将数据从文件写入BUFF
            if (!fork())
            {
                execvp("vim", arg);
            }
            wait(NULL);
            file_write(name); //将数据从BUFF写入文件
        }
        else
        {
            printf("%s command not found\n", command);
        }
    }
    close_fs();

    fclose(Disk);
    return 0;
}