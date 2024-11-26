#include "irtransmit.h"

int main(void) {
  timer3_init();
  DaikinState daikin_state = {.mode = DaikinModeHeat, .power_state = true};
  daikin_enqueue_daikin_state(daikin_state);

  while (1) {
  }
  timer3_deinit();
  return 1;
}
