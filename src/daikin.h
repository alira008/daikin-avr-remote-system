#ifndef __DAIKIN_H__
#define __DAIKIN_H__

#include <stdbool.h>
#include <stdint.h>

// daikin requirements
#define HIGH_STATE_DURATION 450     // 450us
#define LOW_ONE_STATE_DURATION 1286 // 1286us
#define LOW_ZERO_STATE_DURATION 420 // 420us

typedef enum DaikinMode {
  DaikinModeAuto = 0,
  DaikinModeDry = 2,
  DaikinModeCold = 3,
  DaikinModeHeat = 4,
  DaikinModeFan = 6
} DaikinMode;

typedef enum DaikinFanSpeed {
  DaikinFanSpeedOne = 0x03,
  DaikinFanSpeedTwo = 0x04,
  DaikinFanSpeedThree = 0x05,
  DaikinFanSpeedFour = 0x06,
  DaikinFanSpeedFive = 0x07,
  DaikinFanSpeedAuto = 0x0A,
  DaikinFanSpeedSilent = 0x0B
} DaikinFanSpeed;

typedef struct {
  uint8_t temperature;
  DaikinMode mode;
  bool on_timer;
  bool off_timer;
  uint16_t timer_delay;
  bool power_state;
  DaikinFanSpeed fan_speed;
  bool vertical_swing_state;
  bool horizontal_swing_state;
  bool powerful_state;
  bool economy_state;
  bool eco_sensing_state;
  bool comfort_state;
} DaikinState;

#define MAX_FRAME_SIZE 0x13
typedef struct {
  uint8_t buf[MAX_FRAME_SIZE];
  uint8_t frame_size;
} Frame;

#define MAX_MESSAGE_FRAMES_SIZE 3
typedef struct {
  Frame frames[MAX_MESSAGE_FRAMES_SIZE];
} Message;

const Message *const daikin_get_current_message();

#endif // !__DAIKIN_H__
