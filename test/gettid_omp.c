#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <omp.h>
#include <sys/syscall.h>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

static void do_something()
{
  int tid = syscall(SYS_gettid);
  (void)tid;
  DEBUG_PRINT("%d,", tid);
}

int main(int argc, char* argv[])
{
  int n_threads = atoi(argv[1]);
  int iters = atoi(argv[2]);
  int i;

  /*printf("n_threads: %d, iters %d", n_threads, iters);*/

  omp_set_num_threads(n_threads);

#pragma omp parallel private(i)
  {
#pragma omp for schedule(guided) nowait
    for (i = 0; i < iters; i++)
      do_something();
  }

  return 0;
}
