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

/*==================[inclusions]=============================================*/
#include "appBoard.h"
#include "efHal_gpio.h"
#include "FreeRTOS.h"
#include "task.h"

/*==================[macros and typedef]=====================================*/

typedef struct
{
    efHal_gpio_id_t idSw;
    efHal_gpio_id_t idLed;
}idSwLed_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static const idSwLed_t idSwLed1 =
{
    .idSw = EF_HAL_GPIO_SW_1,
    .idLed = EF_HAL_GPIO_LED_GREEN,
};

static const idSwLed_t idSwLed2 =
{
    .idSw = EF_HAL_GPIO_SW_3,
    .idLed = EF_HAL_GPIO_LED_RED,
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void blinky_task(void *pvParameters)
{
    idSwLed_t *p_idSwLed = pvParameters;

    efHal_gpio_confInt(p_idSwLed->idSw, EF_HAL_GPIO_INT_TYPE_FALLING_EDGE);

    for (;;)
    {
        efHal_gpio_waitForInt(p_idSwLed->idSw, portMAX_DELAY);

        efHal_gpio_setPin(p_idSwLed->idLed, false);
        vTaskDelay(pdMS_TO_TICKS(300));
        efHal_gpio_setPin(p_idSwLed->idLed, true);
    }
}

/*==================[external functions definition]==========================*/
int main(void)
{
    appBoard_init();

    xTaskCreate(blinky_task, "blinky_task1", 100, (void * const)&idSwLed1, 0, NULL);

    xTaskCreate(blinky_task, "blinky_task2", 100, (void * const)&idSwLed2, 0, NULL);

    vTaskStartScheduler();
    for (;;);
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (1);
}

/*==================[end of file]============================================*/
