#ifndef COMMUTATION_H
#define COMMUTATION_H

#include "FreeRTOS.h"
#include "semphr.h"
#include <stdint.h>

/* Semaphore given by Hall ISR, taken by commutation task */
extern SemaphoreHandle_t hall_semaphore;

/* Call once from tasks_init() to create commutation task */
void commutation_task(void *args);

#endif
