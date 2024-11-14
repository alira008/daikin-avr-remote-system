#ifndef __TASK_H__
#define __TASK_H__

#include <avr/io.h>
typedef struct {
    uint16_t elapsed_time;
    uint16_t period;
} Task;

#endif
