#include "timer.h"
#include <avr/io.h>

int main(void) {
  timer1_init();

  PORTB |= (1 << PB1);
  while (1) {
    while (!g_timer1_flag)
      ;
    g_timer1_flag = 0;
  }
  return 1;
}
