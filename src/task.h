#ifndef __TASK_H__
#define __TASK_H__

#include <avr/io.h>

typedef struct {
  uint16_t period;
  uint16_t elapsed_time;
} Task;

#endif
