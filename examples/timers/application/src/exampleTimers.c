/*
###############################################################################
#
# Copyright 2021, Gustavo Muro
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

/*==================[inclusions]=============================================*/
#include "appBoard.h"
#include "efHal_gpio.h"
#include "efHal_analog.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/*==================[macros and typedef]=====================================*/

#define THRESHOLD   3900

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static TimerHandle_t timerHandle;
static TaskHandle_t taskHandle;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void timerCallback(TimerHandle_t xTimer)
{
    xTaskNotifyGive(taskHandle);
}

static void blinky_task(void *pvParameters)
{
    int32_t adcRead;

    timerHandle = xTimerCreate("timer",
                200 / portTICK_PERIOD_MS,
                pdTRUE,
                0, timerCallback);

    xTimerStart(timerHandle, portMAX_DELAY);

    efHal_analog_confAsAnalog(EF_HAL_LIGHT_SENSOR);

    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        efHal_analog_startWaitRead(EF_HAL_LIGHT_SENSOR, &adcRead, portMAX_DELAY);

        if(adcRead < THRESHOLD)
            efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, true);
        else
            efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, false);

        efHal_gpio_togglePin(EF_HAL_GPIO_LED_RED);
    }
}

/*==================[external functions definition]==========================*/
void app_init(void)
{
    xTaskCreate(blinky_task, "blinky_task", 100, NULL, 0, &taskHandle);
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (1);
}

extern void vApplicationMallocFailedHook( void )
{
    while (1);
}

/*==================[end of file]============================================*/
