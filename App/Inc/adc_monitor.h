#ifndef ADC_MONITOR_H
#define ADC_MONITOR_H

#include "FreeRTOS.h"
#include "event_groups.h"

/* Event group shared between ADC monitor and fault manager */
extern EventGroupHandle_t fault_event_group;

/* Fault bit definitions - same in both adc_monitor.c and fault_manager.c */
#define FAULT_BIT_OVERCURRENT    (1 << 0)
#define FAULT_BIT_STALL          (1 << 1)
#define FAULT_BIT_UNDERVOLTAGE   (1 << 2)

/* FreeRTOS task function */
void adc_monitor_task(void *args);

#endif
