#ifndef __DAIKIN_H__
#define __DAIKIN_H__

#include <stdbool.h>
#include <stdint.h>

// daikin requirements
#define HIGH_STATE_DURATION 450     // 450us
#define LOW_ONE_STATE_DURATION 1286 // 1286us
#define LOW_ZERO_STATE_DURATION 420 // 420us

typedef enum DaikinMode {
  DaikinModeAuto,
  DaikinModeDry,
  DaikinModeCold,
  DaikinModeHeat,
  DaikinModeFan
} DaikinMode;

typedef enum DaikinFanSpeed {
  DaikinFanSpeedOne,
  DaikinFanSpeedTwo,
  DaikinFanSpeedThree,
  DaikinFanSpeedFour,
  DaikinFanSpeedFive,
  DaikinFanSpeedAuto,
  DaikinFanSpeedSilent
} DaikinFanSpeed;

typedef struct {
  uint8_t temperature;
  DaikinMode mode;
  bool on_timer;
  bool off_timer;
  uint16_t timer_delay;
  bool power_state;
  DaikinFanSpeed fan_speed;
  bool swing_state;
  bool powerful_state;
  bool economy_state;
  bool comfort_state;
} DaikinState;

#endif // !__DAIKIN_H__
