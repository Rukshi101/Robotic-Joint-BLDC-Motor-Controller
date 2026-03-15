#include "FreeRTOS.h"
#include <stddef.h>
/* Definitions of extern variables referenced by velocity_control.c */
SemaphoreHandle_t hall_semaphore = NULL;
QueueHandle_t setpoint_queue = NULL;
