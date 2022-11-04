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
#include "efHal_analog.h"
#include "efHal_internal.h"

/*==================[macros and typedef]=====================================*/

typedef struct
{
    TaskHandle_t taskHandle;
    efHal_gpio_id_t gpioId;
}taskHandle_analog_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static taskHandle_analog_t taskHandle_analog[EF_HAL_ANALOG_TOTAL_WAIT_CONV];
static efHal_analog_callBacks_t callBacks;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void efHal_analog_init(void)
{
    int i;

    for (i = 0 ; i < EF_HAL_ANALOG_TOTAL_WAIT_CONV ; i++)
    {
        taskHandle_analog[i].taskHandle = NULL;
        taskHandle_analog[i].gpioId = EF_HAL_INVALID_ID;
    }
}

extern void efHal_analog_confAsAnalog(efHal_gpio_id_t id)
{
    if (callBacks.confAsAnalog != NULL)
        callBacks.confAsAnalog(id);
    else
    {
        efErrorHdl_error(EF_ERROR_HDL_NULL_POINTER, "callBacks.confAsAnalog");
    }
}

extern bool efHal_analog_startConv(efHal_gpio_id_t id)
{
    bool ret = false;

    if (callBacks.startConv != NULL)
        ret = callBacks.startConv(id);
    else
    {
        efErrorHdl_error(EF_ERROR_HDL_NULL_POINTER, "callBacks.startConv");
    }

    return ret;
}

extern bool efHal_analog_waitConv(efHal_gpio_id_t id, TickType_t xBlockTime)
{
    int i;
    bool ret = false;

    for (i = 0 ; i < EF_HAL_ANALOG_TOTAL_WAIT_CONV ; i++)
    {
        if (taskHandle_analog[i].taskHandle == NULL)
        {
            taskHandle_analog[i].gpioId = id;
            taskHandle_analog[i].taskHandle = xTaskGetCurrentTaskHandle();
            xTaskNotifyStateClear(taskHandle_analog[i].taskHandle);
            break;
        }
    }

    /* check if no free slot error */
    if (i >= EF_HAL_ANALOG_TOTAL_WAIT_CONV)
    {
        efErrorHdl_error(EF_ERROR_HDL_NO_FREE_SLOT, "waitConv");
    }
    else
    {
        if (ulTaskNotifyTake(pdTRUE, xBlockTime))
            ret = true;

        taskHandle_analog[i].gpioId = EF_HAL_INVALID_ID;
        taskHandle_analog[i].taskHandle = NULL;
    }

    return ret;
}

extern int32_t efHal_analog_read(efHal_gpio_id_t id)
{
    int32_t ret = 0;

    if (callBacks.aRead != NULL)
        ret = callBacks.aRead(id);
    else
    {
        efErrorHdl_error(EF_ERROR_HDL_NULL_POINTER, "callBacks.aRead");
    }

    return ret;
}


extern void efHal_internal_analog_setCallBacks(efHal_analog_callBacks_t cb)
{
    callBacks = cb;
}

extern void efHal_internal_analog_endConvInterruptRoutine(efHal_gpio_id_t id)
{
    int i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    for (i = 0 ; i < EF_HAL_ANALOG_TOTAL_WAIT_CONV ; i++)
    {
        if (taskHandle_analog[i].gpioId == id)
        {
            if (taskHandle_analog[i].taskHandle != NULL)
            {
                vTaskNotifyGiveFromISR(taskHandle_analog[i].taskHandle, &xHigherPriorityTaskWoken);
            }
            else
            {
                efErrorHdl_error(EF_ERROR_HDL_NULL_POINTER, "taskHandle");
            }
            break;
        }
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/*==================[end of file]============================================*/
