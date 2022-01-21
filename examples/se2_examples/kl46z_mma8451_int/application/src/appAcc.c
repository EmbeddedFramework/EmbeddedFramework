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
#include "mma8451.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void acc_task(void *pvParameters)
{
    mma8451_accIntCount_t accIntCount;

    appBoard_init();

    for (;;)
    {
        efHal_gpio_waitForInt(EF_HAL_INT1_ACCEL, portMAX_DELAY);

        accIntCount = mma8451_getAccIntCount();

        if (accIntCount.accX > 50)
            efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, true);
        else
            efHal_gpio_setPin(EF_HAL_GPIO_LED_GREEN, false);

        if (accIntCount.accX < -50)
            efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, true);
        else
            efHal_gpio_setPin(EF_HAL_GPIO_LED_RED, false);
    }
}

/*==================[external functions definition]==========================*/
int main(void)
{
    if (xTaskCreate(acc_task, "acc_task", 100, NULL, 0, NULL) != pdPASS)
    {
        while (1);
    }

    vTaskStartScheduler();
    for (;;);
}

/*==================[end of file]============================================*/
