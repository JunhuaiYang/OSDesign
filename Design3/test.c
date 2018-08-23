// 测试设备

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int testdev;
    int i;
    char buf[10];
    testdev = open("/dev/mydev_yjh",O_RDWR);
    if ( testdev == -1 )
    {
          printf("Cann't open file \n");
          exit(0);
     }
     read(testdev,buf,30);
     printf("%s\n",buf);
     close(testdev);
}   