/*
###############################################################################
#
# Copyright 2022, Gustavo Muro
# All rights reserved
#
# This file is part of EmbeddedFirmware.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#                                                                             */

/* Remark: change the next macros, and evaluate the execution
 * configUSE_PREEMPTION
 * configUSE_TIME_SLICING
 * configIDLE_SHOULD_YIELD
 *
 */

/*==================[inclusions]=============================================*/
#include "appBoard.h"
#include "efHal_gpio.h"
#include "FreeRTOS.h"
#include "task.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void task1(void *pvParameters)
{
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        /* do something */
        efHal_gpio_togglePin(EF_HAL_GPIO_LED_GREEN);
    }
}

static void task2(void *pvParameters)
{
    int i;

    for (;;)
    {
        i = 1000000;
        while (i--);

        /* do something */
        efHal_gpio_togglePin(EF_HAL_GPIO_LED_RED);
    }
}
/*==================[external functions definition]==========================*/
int main(void)
{
    appBoard_init();

    xTaskCreate(task1, "task1", 300, NULL, 1, NULL);
    xTaskCreate(task2, "task2", 300, NULL, 0, NULL);

    vTaskStartScheduler();
    for (;;);
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (1);
}

/*==================[end of file]============================================*/
