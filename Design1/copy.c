#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MEMORY_SIZE 100   //后面3个字符用于标志是否读写完成
#define READ  buffer[MEMORY_SIZE]  //读标志
#define WRITE buffer[MEMORY_SIZE+1] //写标志
#define FLAG  buffer[MEMORY_SIZE+2]  //读写完成标志

//由于sys/sem.h 与 linux/sem.h有冲突，所以单独定义
/* arg for semctl system calls. */
union semun {
  int val;			/* value for SETVAL */
  struct semid_ds *buf;	/* buffer for IPC_STAT & IPC_SET */
  unsigned short *array;	/* array for GETALL & SETALL */
  struct seminfo *__buf;	/* buffer for IPC_INFO */
  void *__pad;
};

int segment_id;
int full, empty, mutex;  //信号灯
pid_t p_read, p_write;

//函数审明
void P(int semid);
void V(int semid);
void sem_ass(int semid ,int val);
void ReadBuf(const char* fname);
void WriteBuf(const char* fname);

int main(int argc, char const *argv[])
{
    int error_state = 10;
    //判断参数是否完整
    if(argc != 3)
    {
        printf("输入参数不正确！\n");
        printf("请输入两个参数，前一个参数为被复制的文件名，后一个参数为目标文件名 ！\n");
        return 0;
    }

    //创建共享内存组
    segment_id = shmget(IPC_PRIVATE, MEMORY_SIZE+3, IPC_CREAT|0666);  //+2是为了用最后2个字符用于进程间通信
    //其中 [MEMORY_SIZE] 用于标志写位置， [MEMORY_SIZE+1] 用于标志读位置  [MEMORY_SIZE+2]标志读写完成

    //共享内存后面的标志量赋值
    char *buffer;
    buffer = (char*)shmat(segment_id, NULL, SHM_R|SHM_W);
    READ = 0;
    WRITE = 0;
    FLAG = 1;

    //创建信号灯及赋值
    full = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666); //将共享缓冲区问题转化为生产者消费者问题
    empty = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    mutex = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

    sem_ass(full, 0);
    sem_ass(empty, MEMORY_SIZE);
    sem_ass(mutex, 1);

    //创建进程
    if( (p_read=fork() )== -1)
    {
        printf("Create Process failed! \n");
        return -1;
    }
    else if(p_read > 0)  //确保在父进程中
    {
        if( (p_write=fork()) == -1)
        {
            printf("Create Process failed! \n");
            return -1;
        }
    }

    if(p_read == 0)
    {
        ReadBuf(argv[2]);
    }
    else if(p_write == 0)
    {
        WriteBuf(argv[1]);
    }

    wait(&error_state); //等待子进程结束
    wait(NULL);
    //printf("%d\n", error_state);
    if(error_state == 256)   //  ?? 为什么exit（1）是256？
        printf("发生错误！请检查文件路径是否正确！\n" );
    else
        printf("Process has finished! \nFILE copy success!\n");

    //删除信号灯
    semctl(full, 0, IPC_RMID);
    semctl(empty, 0, IPC_RMID);
    semctl(mutex, 0, IPC_RMID);

    //解绑内存
    shmdt(buffer);
    //释放内存
    shmctl(segment_id, IPC_RMID, 0);

    return 0;
}

void ReadBuf(const char* fname)
{
    char *buffer;
    char p;
    int fd = 0;
    //绑定内存
    buffer = (char*)shmat(segment_id, NULL, SHM_R|SHM_W);
    //buffer[MEMORY_SIZE] 用于标志写位置， buffer[MEMORY_SIZE+1] 用于标志读位置
    //buffer[MEMORY_SIZE+2]标志读写完成 若为1则标志未完成，若为0则标志读写完成

    //打开需要复制的目标文件
    fd = open(fname, O_WRONLY|O_APPEND|O_CREAT);  //以二进制写入
    if(fd == -1)
    {
        printf("文件创建失败！\n");
        exit(1) ;
    }

    //while(FLAG)  //FALG 为读写完成标记
    while(1)   //判断条件转移到while内部
    {
        if(FLAG == 1 && READ == WRITE);  //初始未开始状态, 用;继续运行下方代码
        else if(FLAG == 0 && READ == WRITE)   //已经写入缓冲区完成 && 队列完成
            break;

        P(full);
        P(mutex);  //互斥锁
        p = buffer[(int)READ]; //获取数据

        READ = (READ + 1)% MEMORY_SIZE; //+1
        //写入目标文件
        write(fd, &p, sizeof(char));

        V(mutex);
        V(empty);
    }
    close(fd);  //关闭文件

    //解绑内存
    shmdt(buffer);

    exit(0);
}

void WriteBuf(const char* fname)
{
    char *buffer;
    char c;
    int fd ;
    //绑定内存
    buffer = (char*)shmat(segment_id, NULL, SHM_R|SHM_W);

    //打开需要复制的原文件
    fd = open(fname, O_RDONLY);
    if(fd == -1)
    {
        printf("文件打开失败！\n");
        exit(1);
    }

    while(read(fd, &c, sizeof(char)) > 0)  //成功读取到数据
    {
        P(empty);
        P(mutex);

        buffer[(int)WRITE] = c;

        WRITE = (WRITE+1)%MEMORY_SIZE;

        V(mutex);
        V(full);
    }
    FLAG = 0;  //读取文件结束，标志0
    close(fd); //关闭文件

    //解绑内存
    shmdt(buffer);

    exit(0);
}


void P(int semid)
{
    struct sembuf sem;
    sem.sem_num = 0;
    sem.sem_op = -1;   //P执行-1操作
    sem.sem_flg = 0;   //这里可以直接为0，没有关系

    semop(semid, &sem, 1);   //信号量操作

    return;
}

void V(int semid)
{
    struct sembuf sem;
    sem.sem_num = 0;
    sem.sem_op = +1;   //V执行+1操作
    sem.sem_flg = 0;   //这里可以直接为0，没有关系

    semop(semid, &sem, 1);   //信号量操作

    return;
}

//信号灯赋值
void sem_ass(int semid ,int val)
{
    union semun arg;
    arg.val = val;
    semctl(semid, 0, SETVAL, arg);
}
