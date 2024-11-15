#ifndef __TIMER_H__
#define __TIMER_H__

#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint8_t g_timer1_flag = 0;

static inline void timer1_init() {
  // settings clear timer on compare (CTC)
  // using prescaler 64
  // this means TCNT1 will count to 125,000 ticks/s
  TCCR1B = _BV(WGM13) | _BV(CS11) | _BV(CS10);

  // compare TCNT1 with this value
  OCR1A = 125;

  // reseting TCNT1
  TCNT1 = 0;

  // enable interrupts for Output Compare A
  TIMSK1 = _BV(OCIE1A);

  // enable interrupts globally
  SREG |= _BV(SREG_I);
}

ISR(TIMER1_COMPA_vect) { g_timer1_flag = 1; }

#endif // !__TIMER_H__
