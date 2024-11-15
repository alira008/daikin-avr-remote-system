#ifndef __TIMER_H__
#define __TIMER_H__

#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint8_t g_timer_flag = 0;

void timer_init() {
  // settings clear timer on compare (CTC)
  // using prescaler 64
  // this means TCNT1 will count to 125,000 ticks/s
  TCCR1B = (1 << WGM13) | (1 << CS11) | (1 << CS10);

  // compare TCNT1 with this value
  OCR1A = 125;

  // reseting TCNT1
  TCNT1 = 0;

  // enable interrupts for Output Compare A
  TIMSK1 = 1 << OCIE1A;

  // enable interrupts globally
  SREG |= 1 << SREG_I;
}

inline void timer_reset() { TCCR1B = 0; }

ISR(TIMER1_COMPA_vect) { g_timer_flag = 1; }

#endif // !__TIMER_H__
