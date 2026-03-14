#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32g4xx.h"

/* Cortex-M4 running at 170MHz */
#define configCPU_CLOCK_HZ                      170000000UL
#define configSYSTICK_CLOCK_HZ                  configCPU_CLOCK_HZ

/* Scheduler tick rate - 1ms per tick */
#define configTICK_RATE_HZ                      1000

/* Task priorities - higher number = higher priority */
#define configMAX_PRIORITIES                    5

/* Minimum stack size in words (not bytes) */
#define configMINIMAL_STACK_SIZE                128

/* Total heap available to FreeRTOS for task stacks and queues */
#define configTOTAL_HEAP_SIZE                   8192

/* Max length of task name string */
#define configMAX_TASK_NAME_LEN                 16

#define configUSE_PREEMPTION                    1
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configUSE_MUTEXES                       1
#define configUSE_SEMAPHORES                    1
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               3
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            128
#define configUSE_EVENT_GROUPS                  1

/* Map FreeRTOS interrupt handlers to CMSIS names */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

/* API functions to include */
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_xSemaphoreGetMutexHolder        1
#define INCLUDE_xTaskGetCurrentTaskHandle       1

/* Tick type - use 32 bit ticks */
#define configTICK_TYPE_WIDTH_IN_BITS    TICK_TYPE_WIDTH_32_BITS

/* Interrupt priority - lowest priority for FreeRTOS syscalls */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    191
#endif
