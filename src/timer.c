#include "timer.h"

volatile uint8_t g_current_frame = 0;
volatile uint8_t g_current_byte = 0;
volatile uint8_t g_current_bit = 0;

void update_current_context(uint8_t frame_size);

// will be used for transmitting ir signals
ISR(TIMER3_COMPA_vect) {
  const Message *const message = daikin_get_current_message();
  if (!message->should_send) {
    return;
  }
  Frame frame = message->frames[g_current_frame];
  uint8_t byte = frame.buf[g_current_byte];

  update_current_context(frame.frame_size);
}

void update_current_context(uint8_t frame_size) {
  if (g_current_frame + 1 >= MAX_MESSAGE_FRAMES_SIZE) {
    g_current_frame = 0;
    g_current_byte = 0;
    g_current_bit = 0;
    return;
  }

  if (g_current_byte + 1 >= MAX_FRAME_SIZE ||
      g_current_byte + 1 >= frame_size) {
    g_current_byte = 0;
    g_current_bit = 0;
    return;
  }
  if (g_current_bit + 1 >= 8) {
    g_current_bit = 0;
    return;
  }

  g_current_frame++;
  g_current_byte++;
  g_current_bit++;
}
