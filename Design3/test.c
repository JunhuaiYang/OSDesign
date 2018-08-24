#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
// 测试设备

int main()
{
    int testdev;
    int i;
    char buf[1024]; //设备缓冲区为1024
    testdev = open("/dev/mydev_yjh", O_RDWR);
    if (testdev == -1)
    {
        printf("设备打开失败！ \n");
        exit(0);
    }
    else
        printf("设备打开成功！  \n");
    printf("当前设备内缓冲区内容为：\n");
    read(testdev, buf, 1024);
    printf("%s\n", buf);

    printf("请输入要输入设备的内容：\n");
    scanf("%s",buf);
    
    write(testdev,buf,sizeof(buf));
    printf("\n写入后设备内缓冲区内容为：\n");
    read(testdev, buf, 1024);
    printf("%s\n", buf);

    close(testdev);
    printf("设备成功关闭！");

}