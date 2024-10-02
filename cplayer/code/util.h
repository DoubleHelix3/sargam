#ifndef UTIL
#define UTIL

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

#define KB 1000
#define MB (1000*KB)
#define GB (1000*MB)

#define KiB 1024
#define MiB (1024*KiB)
#define GiB (1024*MiB)


float timer();
void  wait(float duration);

int random_int(int a, int b);

#endif
