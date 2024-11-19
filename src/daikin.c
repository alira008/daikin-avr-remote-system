#include "daikin.h"
#include <avr/io.h>
#include <avr/sfr_defs.h>

// frame helpers
#define INITIATE_FRAME_BUF_HEADER(frame)                                       \
  do {                                                                         \
    frame[0] = 0x11;                                                           \
    frame[1] = 0xDA;                                                           \
    frame[2] = 0x27;                                                           \
    frame[3] = 0x00;                                                           \
  } while (0);

#define CODE_OFFSET 0x04
// Used in Frame 1
#define COMFORT_STATE_OFFSET 0x06
// Used in Frame 3
#define MODE_ON_OFF_TIMER_OFFSET 0x05
#define TEMPERATURE_OFFSET 0x06
#define FAN_VERTICAL_SWING_OFFSET 0x08
#define HORIZONTAL_SWING_OFFSET 0x09
#define TIMER_DELAY_ON_OFFSET_1 0x0A
#define TIMER_DELAY_ON_OFFSET_2 0x0B
#define TIMER_DELAY_OFF_OFFSET_1 0x0B
#define TIMER_DELAY_OFF_OFFSET_2 0x0C
#define POWERFUL_STATE_OFFSET 0x0D
#define ECONOMY_STATE_ECO_SENSING_STATE_OFFSET 0x10
// end frame helpers

// globals
#define MAX_MESSAGES_SIZE 3
uint8_t g_current_message_index = 0;
Message g_messages[MAX_MESSAGES_SIZE] = {0};
// end globals

Frame create_frame_1(bool comfort_state);
Frame create_frame_2();
Frame create_frame_3(DaikinState daikin_state);
uint8_t calculate_checksum(uint8_t *buf, uint8_t size);

const Message *const daikin_get_current_message() {
  return &g_messages[g_current_message_index];
}

void daikin_ack_current_message() {
  if (g_current_message_index + 1 >= MAX_MESSAGES_SIZE) {
    g_current_message_index = 0;
  } else {
    g_current_message_index++;
  }
}

Message convert_daikin_state_to_message(DaikinState daikin_state) {
  Frame frame_1 = create_frame_1(daikin_state.comfort_state);
  Frame frame_2 = create_frame_2();
  Frame frame_3 = create_frame_3(daikin_state);
  return (Message){.frames = {frame_1, frame_2, frame_3}};
}

Frame create_frame_1(bool comfort_state) {
  Frame frame = {.frame_size = 8};
  INITIATE_FRAME_BUF_HEADER(frame.buf);
  // code is 0xC5
  frame.buf[CODE_OFFSET] = 0xC5;
  frame.buf[COMFORT_STATE_OFFSET] = comfort_state ? 0x10 : 0x00;
  frame.buf[7] = calculate_checksum(frame.buf, frame.frame_size - 1);

  return frame;
}

// this frame does not change
Frame create_frame_2() {
  Frame frame = {.frame_size = 8};
  INITIATE_FRAME_BUF_HEADER(frame.buf);
  // code is 0x42
  frame.buf[4] = 0x42;
  frame.buf[7] = 0x54;

  return frame;
}

Frame create_frame_3(DaikinState daikin_state) {
  Frame frame = {.frame_size = MAX_FRAME_SIZE};
  INITIATE_FRAME_BUF_HEADER(frame.buf);
  // code is 0x00
  frame.buf[MODE_ON_OFF_TIMER_OFFSET] = (daikin_state.mode << 4);
  // always on
  frame.buf[MODE_ON_OFF_TIMER_OFFSET] |= _BV(3);
  // timer off enabled
  if (daikin_state.off_timer) {
    frame.buf[MODE_ON_OFF_TIMER_OFFSET] |= _BV(2);
  }
  // timer on enabled
  if (daikin_state.on_timer) {
    frame.buf[MODE_ON_OFF_TIMER_OFFSET] |= _BV(1);
  }
  // current power state
  if (daikin_state.power_state) {
    frame.buf[MODE_ON_OFF_TIMER_OFFSET] |= 1;
  }

  // setting temperature
  frame.buf[TEMPERATURE_OFFSET] = daikin_state.temperature * 2;
  // fan speed
  frame.buf[FAN_VERTICAL_SWING_OFFSET] = (daikin_state.fan_speed << 4);
  // vertical swing
  if (daikin_state.vertical_swing_state) {
    frame.buf[FAN_VERTICAL_SWING_OFFSET] |= 0x0F;
  }
  // horizontal swing
  if (daikin_state.horizontal_swing_state) {
    frame.buf[HORIZONTAL_SWING_OFFSET] |= 0x0F;
  }

  // setting timer for on
  if (daikin_state.on_timer) {
    frame.buf[TIMER_DELAY_ON_OFFSET_1] = (daikin_state.timer_delay >> 4);
    frame.buf[TIMER_DELAY_ON_OFFSET_2] = (0x0F & daikin_state.timer_delay);
  }
  // setting timer for off
  if (daikin_state.off_timer) {
    frame.buf[TIMER_DELAY_OFF_OFFSET_1] = (daikin_state.timer_delay >> 4);
    frame.buf[TIMER_DELAY_OFF_OFFSET_2] = (0x0F & daikin_state.timer_delay);
  }
  // setting powerful state
  if (daikin_state.powerful_state) {
    frame.buf[POWERFUL_STATE_OFFSET] = 0x01;
  }
  // setting economy_state and eco_sensing_state
  if (daikin_state.economy_state) {
    frame.buf[POWERFUL_STATE_OFFSET] |= 0x84;
  }
  if (daikin_state.eco_sensing_state) {
    frame.buf[POWERFUL_STATE_OFFSET] |= 0x82;
  }
  frame.buf[0x12] = calculate_checksum(frame.buf, frame.frame_size - 1);

  return frame;
}

uint8_t calculate_checksum(uint8_t *buf, uint8_t size) {
  uint16_t sum = 0;
  for (uint8_t i = 0; i < size; i++) {
    sum += buf[i];
  }

  return (uint8_t)(sum & 0xFF);
}
