#ifndef VELOCITY_CONTROL_H
#define VELOCITY_CONTROL_H

#include <stdint.h>

/* FreeRTOS task function - register with xTaskCreate */
void velocity_control_task(void *args);

/* Set target RPM - called from CAN task when setpoint received */
void velocity_set_target(float rpm);

/* Get current measured RPM - called from CAN task for telemetry */
float velocity_get_actual(void);

#endif
