#include "irreceive.h"
#include "daikin.h"
#include <string.h>

volatile Message g_current_message = {0};
volatile uint16_t g_current_frame = 0;
volatile uint16_t g_current_byte = 0;
volatile uint16_t g_current_bit = 0;
volatile bool g_high_state = true;

bool update_current_context(uint8_t frame_size);
void reset_current_context();

// we must go from high state -> low 1 state
// we must go from high state -> low 0 state
ISR(TIMER1_CAPT_vect) {
  uint16_t pulse_width = ICR1;

  // toggling interrupt on rising edge or falling edge
  if (TCCR1B & (1 << ICES1)) {
    // switch to falling edge
    TCCR1B &= ~(1 << ICES1);
  } else {
    // verify that pulse width is correct for the high_state
    if (g_high_state && !(pulse_width >= 424 && pulse_width <= 500)) {
      return reset_current_context();
    }
    // verify that pulse width is correct for the low_state
    if (!g_high_state && !((pulse_width >= 1264 && pulse_width <= 1300) ||
                           (pulse_width >= 396 && pulse_width <= 436))) {
      return reset_current_context();
    }

    if (!g_high_state) {
      bool low_state_1;
      if (!g_high_state && pulse_width >= 1264 && pulse_width <= 1300) {
        low_state_1 = true;
      } else {
        low_state_1 = false;
      }

      volatile Frame *frame = &g_current_message.frames[g_current_frame];
      volatile uint8_t *byte = &frame->buf[g_current_byte];
      if (low_state_1) {
        *byte |= _BV(g_current_bit);
      } else {
        *byte &= ~_BV(g_current_bit);
      }

      if (update_current_context(frame->frame_size)) {
        daikin_enqueue_message(g_current_message);
        reset_current_context();
      }
    }

    // switch to rising edge
    TCCR1B |= _BV(ICES1);
    g_high_state = !g_high_state;
  }

  TCNT1 = 0;
}

bool update_current_context(uint8_t frame_size) {
  if (g_current_frame + 1 >= MAX_MESSAGE_FRAMES_SIZE) {
    g_current_frame = 0;
    g_current_byte = 0;
    g_current_bit = 0;
    return true;
  }

  if (g_current_byte + 1 >= MAX_FRAME_SIZE ||
      g_current_byte + 1 >= frame_size) {
    g_current_byte = 0;
    g_current_bit = 0;
    return false;
  }
  if (g_current_bit + 1 >= 8) {
    g_current_bit = 0;
    return false;
  }

  g_current_frame++;
  g_current_byte++;
  g_current_bit++;
  return false;
}

void reset_current_context() {
  g_current_frame = 0;
  g_current_byte = 0;
  g_current_bit = 0;
  g_high_state = true;
  memset(&g_current_message, 0, sizeof(Message));
}
