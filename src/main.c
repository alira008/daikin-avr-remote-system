#include "irreceive.h"
#include "irtransmit.h"

int main(void) {
  ir_transmit_init();
  ir_receive_init();
  DaikinState daikin_state = {.mode = DaikinModeHeat, .power_state = true};
  daikin_enqueue_daikin_state(daikin_state);

  while (1) {
  }
  ir_transmit_deinit();
  ir_receive_deinit();
  return 1;
}
