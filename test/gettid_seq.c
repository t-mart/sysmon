#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

/*#include "timetests.h"*/
static __inline__ unsigned long long tick()
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#define NSEC(timespec) ((timespec).tv_sec * 1000000000 + (timespec).tv_nsec)

int main(int argc, char* argv[])
{


  //systems calls are super fast...faster than a nanonsecond. we can't
  //get any better resolution than that, so instead of counting single
  //calls, we count groups of them: iters_per test
  int iters_per_test = atoi(argv[1]);

  int i, j;
  struct timespec before, after;
  unsigned long long bt, at;

#ifdef TSC
    bt = tick();
#else
    clock_gettime(CLOCK_MONOTONIC, &before);
#endif

    //the action
    for (;j < iters_per_test; j++){
      getpid();
      syscall(SYS_gettid);
      printf("\0");
    }

#ifdef TSC
    at = tick();
#else
    syscall(SYS_clock_gettime, CLOCK_MONOTONIC, &after);
#endif


#ifdef TSC
    printf("%llu\n", at - bt);
#else
    printf("%ld\n", NSEC(after) - NSEC(before));
#endif

    fflush(stdout);

  return 0;
}
