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
#include "efHal_gpio.h"
#include "efHal_internal.h"

/*==================[macros and typedef]=====================================*/

typedef struct
{
    efHal_gpio_callBackInt_t cbInt;
    efHal_gpio_id_t gpioId;
}cb_gpio_t;

typedef struct
{
    TaskHandle_t taskHandle;
    efHal_gpio_id_t gpioId;
}taskHandle_gpio_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static efHal_gpio_callBacks_t callBacks;
static cb_gpio_t cb_gpio[EF_HAL_GPIO_TOTAL_CALL_BACK];
static taskHandle_gpio_t taskHandle_gpio[EF_HAL_GPIO_TOTAL_WAIT_FOR_INT];

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void efHal_gpio_init(void)
{
    int i;
    for (i = 0 ; i < EF_HAL_GPIO_TOTAL_CALL_BACK ; i++)
    {
        cb_gpio[i].cbInt = NULL;
        cb_gpio[i].gpioId = 0;
    }
}

extern void efHal_gpio_setPin(efHal_gpio_id_t id, bool state)
{
    if (callBacks.setPin != NULL)
        callBacks.setPin(id, state);
    else
    {
        /* TODO ASSERT */
    }
}

extern void efHal_gpio_togglePin(efHal_gpio_id_t id)
{
    if (callBacks.togPin != NULL)
        callBacks.togPin(id);
    else
    {
        bool state;
        vPortEnterCritical();
        state = efHal_gpio_getPin(id);
        efHal_gpio_setPin(id, !state);
        vPortExitCritical();
    }
}

extern bool efHal_gpio_getPin(efHal_gpio_id_t id)
{
    bool ret = 0;

    if (callBacks.getPin != NULL)
        ret = callBacks.getPin(id);
    else
    {
        /* TODO ASSERT */
    }

    return ret;
}

extern void efHal_gpio_confInt(efHal_gpio_id_t id, efHal_gpio_intType_t intType)
{
    if (callBacks.confInt != NULL)
        callBacks.confInt(id, intType);
    else
    {
        /* TODO ASSERT */
    }
}

extern void efHal_gpio_confPin(efHal_gpio_id_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull, bool state)
{
    if (callBacks.confPin != NULL)
        callBacks.confPin(id, dir, pull, state);
    else
    {
        /* TODO ASSERT */
    }
}

extern bool efHal_gpio_waitForInt(efHal_gpio_id_t id, TickType_t xBlockTime)
{
    int i;
    bool ret = false;

    for (i = 0 ; i < EF_HAL_GPIO_TOTAL_WAIT_FOR_INT ; i++)
    {
        if (taskHandle_gpio[i].taskHandle == NULL)
        {
            taskHandle_gpio[i].gpioId = id;
            taskHandle_gpio[i].taskHandle = xTaskGetCurrentTaskHandle();
            xTaskNotifyStateClear(taskHandle_gpio[i].taskHandle);
            break;
        }
    }

    /* check if no free slot error */
    if (i >= EF_HAL_GPIO_TOTAL_WAIT_FOR_INT)
    {
        /* TODO ASSERT */
    }

    if (ulTaskNotifyTake(pdTRUE, xBlockTime))
        ret = true;

    taskHandle_gpio[i].taskHandle = NULL;

    return ret;
}

extern void efHal_gpio_setCallBackInt(efHal_gpio_id_t id, efHal_gpio_callBackInt_t cb)
{
    int i;
    for (i = 0 ; i < EF_HAL_GPIO_TOTAL_CALL_BACK ; i++)
    {
        if (cb_gpio[i].cbInt == NULL)
        {
            cb_gpio[i].gpioId = id;
            cb_gpio[i].cbInt = cb;
            break;
        }
    }

    /* check if no free slot error */
    if (i >= EF_HAL_GPIO_TOTAL_CALL_BACK)
    {
        /* TODO ASSERT */
    }
}

extern void efHal_internal_gpio_setCallBacks(efHal_gpio_callBacks_t cb)
{
    callBacks = cb;
}

extern void efHal_internal_gpio_InterruptRoutine(efHal_gpio_id_t id)
{
    int i;

    for (i = 0 ; i < EF_HAL_GPIO_TOTAL_CALL_BACK ; i++)
    {
        if (cb_gpio[i].gpioId == id)
        {
            if (cb_gpio[i].cbInt != NULL)
            {
                cb_gpio[i].cbInt(id);
            }
            else
            {
                /* TODO ASSERT */
            }

            break;
        }
    }

    for (i = 0 ; i < EF_HAL_GPIO_TOTAL_WAIT_FOR_INT ; i++)
    {
        if (taskHandle_gpio[i].gpioId == id)
        {
            if (taskHandle_gpio[i].taskHandle != NULL)
            {
                xTaskNotifyGive(taskHandle_gpio[i].taskHandle);
            }
            else
            {
                /* TODO ASSERT */
            }

            break;
        }
    }
}

/*==================[end of file]============================================*/
