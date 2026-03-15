#include "fault_manager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "stm32g4xx_hal.h"

/* TIM1 handle for PWM disable on fault */
extern TIM_HandleTypeDef htim1;

/* Fault event group defined in adc_monitor.c */
extern EventGroupHandle_t fault_event_group;

/* How many times to attempt recovery before permanent shutdown */
#define MAX_RECOVERY_ATTEMPTS    3

/* Fault manager task - blocks on event group, handles fault recovery */
void fault_manager_task(void *args) {
    uint8_t recovery_attempts = 0;
    EventBits_t fault_bits;

    while (1) {
        /* Block here until any fault bit is set
           This is the blocking call that justifies FreeRTOS -
           waiting for multiple event sources simultaneously
           cannot be done cleanly in an ISR or state machine */
        fault_bits = xEventGroupWaitBits(
            fault_event_group,
            FAULT_BIT_OVERCURRENT | FAULT_BIT_STALL | FAULT_BIT_UNDERVOLTAGE,
            pdTRUE,   /* clear bits on exit */
            pdFALSE,  /* any bit triggers, not all */
            portMAX_DELAY
        );

        /* Disable all PWM outputs immediately */
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
        HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);

        recovery_attempts++;

        if (recovery_attempts >= MAX_RECOVERY_ATTEMPTS) {
            /* Permanent shutdown - too many faults */
            while (1) {
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }

        /* Wait 500ms before attempting recovery
           This blocking delay is impossible in an ISR -
           another justification for FreeRTOS task architecture */
        vTaskDelay(pdMS_TO_TICKS(500));

        /* Re-enable PWM and resume operation */
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    }
}
