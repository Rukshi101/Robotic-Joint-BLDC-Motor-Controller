#ifndef STM32G4XX_HAL_H
#define STM32G4XX_HAL_H

#include <stdint.h>

/* Minimal stubs so velocity_control.c compiles on PC */
typedef struct { void *Instance; } TIM_HandleTypeDef;
#define __HAL_TIM_GET_COUNTER(h)     (0U)
#define __HAL_TIM_SET_COMPARE(h,c,v) do {} while(0)

#endif
