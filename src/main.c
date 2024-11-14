#include "timer.h"
#include <avr/io.h>

int main(void) {
  timer_init();

  PORTB |= (1 << PB1);
  while (1) {
    while (!g_timer_flag)
      ;
    g_timer_flag = 0;
  }

  timer_reset();
  return 1;
}
