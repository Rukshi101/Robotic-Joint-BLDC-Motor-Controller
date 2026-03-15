#include "velocity_control.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stm32g4xx_hal.h"
#include "queue.h"
#include "can_telemetry.h"


/* TIM1 handle for PWM duty cycle updates */
extern TIM_HandleTypeDef htim1;

/* TIM4 handle for Hall edge timestamps */
extern TIM_HandleTypeDef htim4;

/* Semaphore shared with commutation task - both wake on Hall edge */
extern SemaphoreHandle_t hall_semaphore;

/* Current velocity setpoint in RPM - written by CAN task via queue */
static volatile float target_rpm = 0.0f;

/* Proportional gain - tuned on bench */
static const float Kp = 0.5f;

/* Motor has 8 poles, 6 Hall edges per electrical revolution
   24 Hall edges per mechanical revolution */
#define HALL_EDGES_PER_REV    24

/* TIM4 runs at 170MHz with no prescaler */
#define TIMER_CLOCK_HZ        170000000UL

/* PWM period register value - must match TIM1 ARR from CubeMX */
#define PWM_PERIOD            4249

/* Clamp duty cycle between 0 and max */
#define CLAMP(x, min, max)   ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/* Update TIM1 PWM duty cycle for all three driven phases */
static void set_duty_cycle(float duty) {
    uint32_t pulse = (uint32_t)(duty * PWM_PERIOD);
    pulse = CLAMP(pulse, 0, PWM_PERIOD);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pulse);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pulse);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pulse);
}

/* Velocity control task - wakes on Hall semaphore, same as commutation */
void velocity_control_task(void *args) {
    uint32_t last_timestamp = 0;
    uint32_t current_timestamp = 0;
    float actual_rpm = 0.0f;
    float error = 0.0f;
    float duty = 0.0f;

    while (1) {
        /* Block until Hall edge - commutation task has higher priority
           so it always runs first on the same semaphore give */
        xSemaphoreTake(hall_semaphore, portMAX_DELAY);

        /* Capture TIM4 counter at this Hall edge */
        current_timestamp = __HAL_TIM_GET_COUNTER(&htim4);

        /* Compute time delta between consecutive Hall edges */
        uint32_t delta = current_timestamp - last_timestamp;
        last_timestamp = current_timestamp;

        /* Avoid division by zero on startup */
        if (delta == 0) continue;

        /* RPM = (timer_clock / delta) / edges_per_rev * 60 */
        actual_rpm = ((float)TIMER_CLOCK_HZ / (float)delta)
                     / (float)HALL_EDGES_PER_REV * 60.0f;

        /* Check for new setpoint from CAN task */
        float new_setpoint;
        if (xQueueReceive(setpoint_queue, &new_setpoint, 0) == pdTRUE) {
            target_rpm = new_setpoint;
        }

        /* Proportional controller - error drives duty cycle */
        error = target_rpm - actual_rpm;
        duty += Kp * error / target_rpm;
        duty = CLAMP(duty, 0.0f, 1.0f);

        set_duty_cycle(duty);
    }
}

/* Called from CAN task to update setpoint */
void velocity_set_target(float rpm) {
    target_rpm = rpm;
}

/* Called from anywhere to read current measured RPM */
float velocity_get_actual(void) {
    return 0.0f; /* placeholder - actual_rpm is local to task */
}
