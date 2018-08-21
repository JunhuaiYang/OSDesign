// 不使用环形缓冲池
// 单进程

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
// #include <time.h>
#include <sys/time.h>

#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{

  //判断参数是否完整
  if (argc != 3)
  {
    printf("输入参数不正确！\n");
    printf("请输入两个参数，前一个参数为被复制的文件名，后一个参数为目标文件名 ！\n");
    return -1;
  }

  struct timeval start, end;
  // 计时开始
  gettimeofday(&start, NULL);

  // 被复制的文件  只读
  int copy_fd = open(argv[1], O_RDONLY);
  if (copy_fd == -1)
  {
    printf("open %s error\n", argv[1]);
    return -1;
  }

  // 目标路径和文件  没有则创建
  int to_fd = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  if (to_fd == -1)
  {
    printf("create %s error\n", argv[2]);
    return -1;
  }

  char buf[BUFF_SIZE]; //一次读写BUFF_SIZE个字节
  while (1)
  {
    // 读
    int Readsum = read(copy_fd, buf, sizeof(buf));
    if (Readsum == -1)
    {
      printf("read %s error\n", argv[1]);
      return -1;
    }
    else if (Readsum > 0)
    {
      // 写
      int WriteRes = write(to_fd, buf, Readsum);
      if (WriteRes != Readsum)
      {
        printf("write %s error\n", argv[2]);
        return -1;
      }
    }
    // 完成
    else if (Readsum == 0)
    {
      printf("copy %s complete\n", argv[1]);
      // 计时结束
      gettimeofday(&end,NULL);
      suseconds_t msec = end.tv_usec-start.tv_usec;
      time_t sec = end.tv_sec-start.tv_sec;
      printf("time used : %lu.%lus\n",sec,msec);

      break;
    }
  }

  return 0;
}
