#include <stdio.h>
#include <stdlib.h>
#include<sys/time.h>

int main ()
{
  for (int i = 0; i < 100; ++i) {
    struct timeval tpstart;
    gettimeofday(&tpstart, NULL);
    srand(tpstart.tv_usec);
    int random = rand();
    printf("%d\n", random);
    random = rand();
    printf("%d\n", random);
    random = rand();
    printf("%d\n", random);
  }
}
