#include "util.h"
#include <time.h>

clock_t timer_start;
bool is_timer_initialized = false;

float timer() {
  if (!is_timer_initialized) {
    timer_start = clock();
    is_timer_initialized = true;
  }

  float timer_current = clock();
  return (timer_current - timer_start) / CLOCKS_PER_SEC;
}

void wait(float duration) {
  float start_time = timer();
  while (timer() - start_time < duration) {}
}



bool is_random_initialized = false;

int random_int(int a, int b) {
  if (!is_random_initialized) {
    srand(clock());
    is_random_initialized = true;
  }

  return a + rand() % (b - a);
}

