#ifndef CAN_TELEMETRY_H
#define CAN_TELEMETRY_H

#include "FreeRTOS.h"
#include "queue.h"

/* Queue for passing velocity setpoints to velocity control task */
extern QueueHandle_t setpoint_queue;

/* FreeRTOS task function */
void can_telemetry_task(void *args);

#endif
