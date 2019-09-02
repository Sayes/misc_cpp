#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main ()
{
  int RANGE_MIN = 1000;
  int RANGE_MAX = 3000;
  for (int i = 0; i < 100; ++i) {
    struct timeval tpstart;
    gettimeofday(&tpstart, NULL);
    srand(tpstart.tv_usec);
    int random = rand();
    printf("%d\n", (int)((random*1.0f/RAND_MAX)*(RANGE_MAX-RANGE_MIN) + RANGE_MIN));
    random = rand();
    printf("%d\n", (int)((random*1.0f/RAND_MAX)*(RANGE_MAX-RANGE_MIN) + RANGE_MIN));
    random = rand();
    printf("%d\n", (int)((random*1.0f/RAND_MAX)*(RANGE_MAX-RANGE_MIN) + RANGE_MIN));
  }
}
