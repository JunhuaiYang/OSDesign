#include <stdio.h>
#include <pthread.h>

// 用于测试cpu

void* count_()
{
    long int i;
    while(1)
        i++;
}

int main(int argc, char const *argv[])
{
    pthread_t p;
    int i;
    for( i = 0; i < 7; i++)
    {
        if(pthread_create(&p, NULL, count_, NULL) == -1)
        {
            printf("Create Pthread 1 Error! \n");
            return -1;
        }
        printf("success!\n");
    }

    pthread_join(p, NULL);

    return 0;
}
