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
#include "FreeRTOS.h"
#include "task.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void gpio_callBackInt(efHal_gpio_id_t id)
{
    switch (id)
    {
        case EF_HAL_GPIO_SW_1:
            efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, false);
            break;

        case EF_HAL_GPIO_SW_3:
            efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, true);
            break;
    }

}

static void blinky_task(void *pvParameters)
{
    efHal_gpio_confInt(EF_HAL_GPIO_SW_1, EF_HAL_GPIO_INT_TYPE_FALLING_EDGE);
    efHal_gpio_confInt(EF_HAL_GPIO_SW_3, EF_HAL_GPIO_INT_TYPE_FALLING_EDGE);

    EF_TRY
    {
        efHal_gpio_setCallBackInt(EF_HAL_GPIO_SW_1, gpio_callBackInt);
        EF_CHECK;
        efHal_gpio_setCallBackInt(EF_HAL_GPIO_SW_1, gpio_callBackInt);
        EF_CHECK;
        efHal_gpio_setCallBackInt(EF_HAL_GPIO_SW_1, gpio_callBackInt);
        EF_CHECK;

        efHal_gpio_setCallBackInt(EF_HAL_GPIO_SW_3, gpio_callBackInt);
        EF_CHECK;
        efHal_gpio_setCallBackInt(EF_HAL_GPIO_SW_3, gpio_callBackInt);
        EF_CHECK;
        efHal_gpio_setCallBackInt(EF_HAL_GPIO_SW_3, gpio_callBackInt);
        EF_CHECK;
    }
    EF_CATCH
    {
        efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, false);
    }
    EF_ENDTRY;


    for (;;)
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        efHal_gpio_togglePin(EF_HAL_GPIO_LED_GREEN);
    }
}

/*==================[external functions definition]==========================*/
int main(void)
{
    appBoard_init();

    if (xTaskCreate(blinky_task, "blinky_task", 100, NULL, 0, NULL) != pdPASS)
    {
        while (1);
    }

    vTaskStartScheduler();
    for (;;);
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (1);
}

/*==================[end of file]============================================*/
