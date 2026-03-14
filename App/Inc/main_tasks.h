#ifndef MAIN_TASKS_H
#define MAIN_TASKS_H

#include <stdint.h>

/* Counters visible in debugger to verify both tasks are running */
//exterm means variable defined somewhere else and lets other files read the counters in debugger
extern volatile uint32_t task1_count;
extern volatile uint32_t task2_count;

/* Call once from main() - starts scheduler, never returns */
void tasks_init(void);

#endif
