#ifndef __TIMER_H__
#define __TIMER_H__

#include "daikin.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

extern volatile bool g_timer_on;

static inline void timer3_init() {
  if (g_timer_on)
    return;

  // normal mode
  TCCR3A = 0;
  // enable ctc mode
  // 8 prescaler
  // this sets our 1us tick
  TCCR3B = _BV(WGM32) | _BV(CS31);

  // start on a high state
  OCR3A = HIGH_STATE_DURATION;

  // reseting TCNT3
  TCNT3 = 0;

  // enable interrupts for Output Compare A
  TIMSK3 = _BV(OCIE3A);

  // enable interrupts globally
  SREG |= _BV(SREG_I);

  g_timer_on = true;
}

static inline void timer3_deinit() {
  // normal mode
  TCCR3A = 0;
  // disable ctc mode
  TCCR3B = 0;

  // start on a high state
  OCR3A = 0;

  // reseting TCNT3
  TCNT3 = 0;

  // disable interrupts for Output Compare A
  TIMSK3 &= ~(1 << OCIE3A);

  // disable interrupts globally
  SREG &= ~(1 << SREG_I);

  g_timer_on = false;
}

#endif // !__TIMER_H__
