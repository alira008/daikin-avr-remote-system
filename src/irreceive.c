#include "irreceive.h"
#include "daikin.h"

volatile Message g_current_message = {0};
volatile uint16_t g_current_frame = 0;
volatile uint16_t g_current_byte = 0;
volatile uint16_t g_current_bit = 0;

// we must go from high state -> low 1 state
// we must go from high state -> low 0 state
ISR(TIMER1_CAPT_vect) {
  uint16_t pulse_width = ICR1;

  // toggling interrupt on rising edge or falling edge
  if (TCCR1B & (1 << ICES1)) {
    // switch to falling edge
    TCCR1B &= ~(1 << ICES1);
  } else {
    // switch to rising edge
    TCCR1B |= _BV(ICES1);
  }

  TCNT1 = 0;
}
