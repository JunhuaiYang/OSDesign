#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
  pid_t p1, p2;
  if ((p1 = fork()) == 0)
  {
    execv("./time.a", NULL);
  }
  else
  {
    if ((p2 = fork()) == 0)
    {
      execv("./shownum.a", NULL);
    }
    else
    {
      execv("./sum.a", NULL);
    }
  }
}