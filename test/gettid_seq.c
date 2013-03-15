#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

void do_something()
{
  int tid = syscall(SYS_gettid);
  DEBUG_PRINT("%d,", tid);
}

int main(int argc, char* argv[])
{
  int iters = atoi(argv[1]);
  int i;

  for (i = 0; i < iters; i++){
    do_something();
  }
}
