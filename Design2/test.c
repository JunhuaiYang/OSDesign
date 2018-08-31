#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    // 调用hello_world
    printf("调用hello_world\n");
    long int callreturn = syscall(333);
    printf("系统调用返回值为 %ld \n", callreturn);

    return 0;
}
