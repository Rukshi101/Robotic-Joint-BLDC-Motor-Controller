#include "FreeRTOS.h"
#include "task.h"

/* Counters incremented by each task - visible in debugger */
volatile uint32_t task1_count = 0;
volatile uint32_t task2_count = 0;

/* Task 1 - high frequency, every 200ms */
void task1_handler(void *args) {
    while (1) {
        task1_count++;
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

/* Task 2 - low frequency, every 800ms */
void task2_handler(void *args) {
    while (1) {
        task2_count++;
        vTaskDelay(pdMS_TO_TICKS(800));
    }
}

/* Call this from main() to create tasks and start scheduler */
void tasks_init(void) {
    xTaskCreate(task1_handler, "Task1", 128, NULL, 2, NULL);
    xTaskCreate(task2_handler, "Task2", 128, NULL, 1, NULL);
    vTaskStartScheduler();
}
