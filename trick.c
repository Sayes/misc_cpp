/*
 * Copyright (c) 2017-2018 S.Y.Z
 *
 * gcc trick.c -o release/trick_c
 */

#include <linux/kernel.h>
#include <stdio.h>
#include <stdlib.h>

#define offsetof(type, member) (size_t) & (((type *)0)->member)
#define container_of(ptr, type, member)                \
  ({                                                   \
    const typeof(((type *)0)->member) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); \
  })

typedef struct sample {
  int id;
  char name[20];
  int age;
} sample;

int main(int argc, char *argv[]) {
  // get struct instance pointer with it's member pointer
  sample *p_sample_ins = (sample *)malloc(sizeof(sample));
  p_sample_ins->age = 10;
  sample *p_sample_tmp = container_of(&(p_sample_ins->age), sample, age);
  printf("p_sample_ins address: %p\n", p_sample_ins);
  printf("p_sample_tmp address: %p\n", p_sample_tmp);
  free(p_sample_ins);
  p_sample_ins = NULL;
  p_sample_tmp = NULL;

  return 0;
}
