#include "commutation.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stm32g4xx_hal.h"

/* Binary semaphore - given by Hall ISR, taken by this task */
SemaphoreHandle_t hall_semaphore;

/* TIM1 handle defined in main.c by CubeMX */
extern TIM_HandleTypeDef htim1;

/* Read the 3 Hall sensor pins and return a 3-bit value
   Bit 0 = H1 (PB6), Bit 1 = H2 (PB7), Bit 2 = H3 (PB8) */
static uint8_t read_hall_state(void) {
    uint8_t h1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) ? 1 : 0;
    uint8_t h2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) ? 1 : 0;
    uint8_t h3 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) ? 1 : 0;
    return (h3 << 2) | (h2 << 1) | h1;
}

/* Set TIM1 channel to PWM mode (high side driven) */
static void set_phase_pwm(uint32_t channel) {
    TIM_OC_InitTypeDef oc = {0};
    oc.OCMode = TIM_OCMODE_PWM1;
    oc.Pulse = htim1.Instance->ARR / 2; /* 50% duty cycle for now */
    oc.OCPolarity = TIM_OCPOLARITY_HIGH;
    oc.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    oc.OCFastMode = TIM_OCFAST_DISABLE;
    oc.OCIdleState = TIM_OCIDLESTATE_RESET;
    oc.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&htim1, &oc, channel);
    HAL_TIM_PWM_Start(&htim1, channel);
}

/* Set TIM1 channel low side on, high side off */
static void set_phase_low(uint32_t channel) {
    HAL_TIM_PWM_Stop(&htim1, channel);
    HAL_TIMEx_PWMN_Start(&htim1, channel);
}

/* Turn off both sides of a phase */
static void set_phase_off(uint32_t channel) {
    HAL_TIM_PWM_Stop(&htim1, channel);
    HAL_TIMEx_PWMN_Stop(&htim1, channel);
}

/* 6-step commutation table indexed by Hall state (1-6)
   Each row: {phase A state, phase B state, phase C state}
   0 = off, 1 = PWM (high), 2 = low */
static const uint8_t commutation_table[8][3] = {
    {0, 0, 0},  /* state 0 - invalid */
    {1, 2, 0},  /* state 1: H1=1 H2=0 H3=0 → A+ B- */
    {0, 1, 2},  /* state 2: H1=0 H2=1 H3=0 → B+ C- */
    {1, 0, 2},  /* state 3: H1=1 H2=1 H3=0 → A+ C- */
    {2, 0, 1},  /* state 4: H1=0 H2=0 H3=1 → C+ A- */
    {2, 1, 0},  /* state 5: H1=1 H2=0 H3=1 → B+ A- */
    {0, 2, 1},  /* state 6: H1=0 H2=1 H3=1 → C+ B- */
    {0, 0, 0},  /* state 7 - invalid */
};

/* Apply commutation step based on Hall state */
static void commutate(uint8_t hall_state) {
    const uint8_t *step = commutation_table[hall_state];

    /* Phase A */
    if      (step[0] == 1) set_phase_pwm(TIM_CHANNEL_1);
    else if (step[0] == 2) set_phase_low(TIM_CHANNEL_1);
    else                   set_phase_off(TIM_CHANNEL_1);

    /* Phase B */
    if      (step[1] == 1) set_phase_pwm(TIM_CHANNEL_2);
    else if (step[1] == 2) set_phase_low(TIM_CHANNEL_2);
    else                   set_phase_off(TIM_CHANNEL_2);

    /* Phase C */
    if      (step[2] == 1) set_phase_pwm(TIM_CHANNEL_3);
    else if (step[2] == 2) set_phase_low(TIM_CHANNEL_3);
    else                   set_phase_off(TIM_CHANNEL_3);
}

/* Commutation task - highest priority, blocks on Hall semaphore */
void commutation_task(void *args) {
    hall_semaphore = xSemaphoreCreateBinary();

    while (1) {
        /* Block here until Hall ISR gives the semaphore */
        xSemaphoreTake(hall_semaphore, portMAX_DELAY);

        /* Read Hall state and commutate immediately */
        uint8_t state = read_hall_state();
        if (state >= 1 && state <= 6) {
            commutate(state);
        }
    }
}
