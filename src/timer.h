#ifndef __TIMER_H__
#define __TIMER_H__

#include "daikin.h"
#include <avr/interrupt.h>
#include <avr/io.h>

static inline void timer3_init() {
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
}

// will be used for transmitting ir signals
ISR(TIMER3_COMPA_vect) {
  const Message *const message = daikin_get_current_message();
  if (!message->should_send) {
    return;
  }
}

#endif // !__TIMER_H__
