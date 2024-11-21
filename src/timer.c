#include "timer.h"
#include "daikin.h"

volatile bool g_timer_on = false;
volatile bool g_is_high_state = false;
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
  uint8_t bit = 0x1 & (byte >> g_current_bit);
  if (!g_is_high_state) {
    // transfer 1
    if (bit) {
      TCNT3 = 0;
      OCR3A = LOW_ONE_STATE_DURATION;
    }
    // transfer 0
    else {
      TCNT3 = 0;
      OCR3A = LOW_ZERO_STATE_DURATION;
    }
    g_is_high_state = true;
  } else {
    TCNT3 = 0;
    OCR3A = HIGH_STATE_DURATION;
    g_is_high_state = false;
  }

  // continue sending messages if we still have most messages to send
  if (g_current_frame + 1 >= MAX_MESSAGE_FRAMES_SIZE) {
    daikin_ack_current_message();
    const Message *const next_message = daikin_get_current_message();
    // if there are no more messages queued to send, stop timer
    if (!next_message->should_send) {
      timer3_deinit();
    }
  }
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
