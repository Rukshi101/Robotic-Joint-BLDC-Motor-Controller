#ifndef FREERTOS_H
#define FREERTOS_H

#include <stdint.h>

typedef void* QueueHandle_t;
typedef void* SemaphoreHandle_t;
typedef uint32_t TickType_t;
typedef int BaseType_t;

#define pdTRUE     1
#define pdFALSE    0
#define portMAX_DELAY 0xFFFFFFFF

static inline int xQueueReceive(QueueHandle_t q, void *buf, uint32_t ticks) {
    return pdFALSE;
}

static inline int xSemaphoreTake(SemaphoreHandle_t s, uint32_t ticks) {
    return pdFALSE;
}

static inline int xSemaphoreGiveFromISR(SemaphoreHandle_t s, int *woken) {
    return pdFALSE;
}

#endif
