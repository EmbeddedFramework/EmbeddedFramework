/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "osal_config.h"

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION                        1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION     1
#define configUSE_IDLE_HOOK                         1
#define configUSE_TICK_HOOK                         1
#define configTICK_RATE_HZ                          ( ( TickType_t ) 1000 )
#define configCPU_CLOCK_HZ                          ( 72000000UL )
#define configMAX_PRIORITIES                        ( OSAL_TOTAL_PRIORITIES )
#define configMINIMAL_STACK_SIZE                    ( 50 )
#define configTOTAL_HEAP_SIZE                       ( ( size_t ) OSAL_HEAP_SIZE )
#define configMAX_TASK_NAME_LEN                     ( 8 )
#define configUSE_TRACE_FACILITY                    0
#define configUSE_16_BIT_TICKS                      0
#define configIDLE_SHOULD_YIELD                     0
#define configUSE_MUTEXES                           1
#define configCHECK_FOR_STACK_OVERFLOW              3
#define configQUEUE_REGISTRY_SIZE                   0
#define configUSE_RECURSIVE_MUTEXES                 0
#define configUSE_MALLOC_FAILED_HOOK                1
#define configUSE_APPLICATION_TASK_TAG              0
#define configUSE_COUNTING_SEMAPHORES               1
#define configGENERATE_RUN_TIME_STATS               0
#define configUSE_STATS_FORMATTING_FUNCTIONS        0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                       0
#define configMAX_CO_ROUTINE_PRIORITIES             ( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS                            0
#define configTIMER_TASK_PRIORITY                   ( 2 )
#define configTIMER_QUEUE_LENGTH                    5
#define configTIMER_TASK_STACK_DEPTH                ( 190 * 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                    0
#define INCLUDE_uxTaskPriorityGet                   0
#define INCLUDE_vTaskDelete                         0
#define INCLUDE_vTaskCleanUpResources               0
#define INCLUDE_vTaskSuspend                        1
#define INCLUDE_vTaskDelayUntil                     1
#define INCLUDE_vTaskDelay                          1
#define INCLUDE_uxTaskGetStackHighWaterMark         0
#define INCLUDE_eTaskGetState                       0
#define INCLUDE_pcTaskGetTaskName                   0


/* Use the system definition, if there is one */
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       __NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       3        /* 8 priority levels */
#endif

/* The lowest priority. */
#define configKERNEL_INTERRUPT_PRIORITY 	( 7 << (8 - configPRIO_BITS) )

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( 1 << (8 - configPRIO_BITS) )



#endif /* FREERTOS_CONFIG_H */

