#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

void do_something()
{
  struct timeval before, after;

  gettimeofday(&before, NULL);
  int tid = syscall(SYS_stat);
  gettimeofday(&after, NULL);

  timersub(&after,&before,&after);
  DEBUG_PRINT("%lld.%06lld\n", after.tv_sec, after.tv_usec);
}

int main(int argc, char* argv[])
{
  int iters = atoi(argv[1]);
  int i;

  for (i = 0; i < iters; i++){
    do_something();
  }
}
