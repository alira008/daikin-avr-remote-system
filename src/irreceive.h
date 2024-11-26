#ifndef __IRRECEIVE_H__
#define __IRRECEIVE_H__

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

extern volatile bool g_ir_receive_on;
extern volatile uint16_t g_current_frame;
extern volatile uint16_t g_current_byte;
extern volatile uint16_t g_current_bit;

static inline void ir_receive_init() {
  if (g_ir_receive_on)
    return;

  // 8 prescalar
  // Input capture on rise edge
  TCCR1B = _BV(CS11) | _BV(ICES1);

  // interrupt mask register
  // interrupt on input capture
  TIMSK1 = _BV(ICIE1);

  // reset TCNT1
  TCNT1 = 0;

  // enable interrupts globally
  SREG |= _BV(SREG_I);

  g_ir_receive_on = true;
  g_current_frame = 0;
  g_current_byte = 0;
  g_current_bit = 0;
}

static inline void ir_receive_deinit() {
  TCCR1B = 0;

  // disable interrupts for input capture and overflow
  TIMSK1 &= ~(1 << ICIE1);
  TIMSK1 &= ~(1 << TOIE1);

  // disable interrupts globally
  SREG &= ~(1 << SREG_I);
}

#endif // !__IRRECEIVE_H__
