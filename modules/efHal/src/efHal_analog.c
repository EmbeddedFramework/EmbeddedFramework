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
static efHal_analog_endConvCB_t endConvCB[EF_HAL_ANALOG_TOTAL_CALL_BACK_CONV];

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
    for (i = 0 ; i < EF_HAL_ANALOG_TOTAL_CALL_BACK_CONV ; i++)
    {
        endConvCB[i]= NULL;
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

extern bool efHal_analog_startWaitRead(efHal_gpio_id_t id, int32_t *result, TickType_t xBlockTime)
{
    bool ret = true;
    int slot;

    /* search for free slot */
    taskENTER_CRITICAL();

    for (slot = 0 ; slot < EF_HAL_ANALOG_TOTAL_WAIT_CONV ; slot++)
    {
        if (taskHandle_analog[slot].gpioId == EF_HAL_INVALID_ID)
        {
            taskHandle_analog[slot].gpioId = id;
            taskHandle_analog[slot].taskHandle = xTaskGetCurrentTaskHandle();
            xTaskNotifyStateClear(taskHandle_analog[slot].taskHandle);
            break;
        }
    }

    taskEXIT_CRITICAL();

    /* check if no free slot error */
    if (slot >= EF_HAL_ANALOG_TOTAL_WAIT_CONV)
    {
        ret = false;
        efErrorHdl_error(EF_ERROR_HDL_NO_FREE_SLOT, "callBacks.startConv");
    }

    /* start conversion */
    if (ret)
        ret = efHal_analog_startConv(id);

    /* wait for end conversion */
    if (ret)
        ret = ulTaskNotifyTake(pdTRUE, xBlockTime);

    if (ret)
    {
        /* read value */
        if (callBacks.aRead != NULL)
            *result = callBacks.aRead(id);
        else
        {
            ret = false;
            efErrorHdl_error(EF_ERROR_HDL_NULL_POINTER, "callBacks.aRead");
        }
    }

    /* release slot */
    taskENTER_CRITICAL();
    if (slot < EF_HAL_ANALOG_TOTAL_WAIT_CONV)
    {
        taskHandle_analog[slot].gpioId = EF_HAL_INVALID_ID;
        taskHandle_analog[slot].taskHandle = NULL;
    }
    taskEXIT_CRITICAL();

    return ret;
}

extern bool efHal_analog_setEndConvCB(efHal_analog_endConvCB_t ecCB)
{
    bool ret = true;
    int slot;

    /* search for free slot */
    taskENTER_CRITICAL();

    for (slot = 0 ; slot < EF_HAL_ANALOG_TOTAL_CALL_BACK_CONV ; slot++)
    {
        if (endConvCB[slot] == NULL)
        {
            endConvCB[slot] = ecCB;
            break;
        }
    }

    taskEXIT_CRITICAL();

    /* check if no free slot error */
    if (slot >= EF_HAL_ANALOG_TOTAL_CALL_BACK_CONV)
    {
        ret = false;
        efErrorHdl_error(EF_ERROR_HDL_NO_FREE_SLOT, "callBacks.setEndConvCB");
    }

    return ret;
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

extern int32_t efHal_analog_getFullValue(efHal_gpio_id_t id)
{
    int32_t ret = 0;

    if (callBacks.getFullValue != NULL)
        ret = callBacks.getFullValue(id);
    else
    {
        efErrorHdl_error(EF_ERROR_HDL_NULL_POINTER, "callBacks.getFullValue");
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
            break;
        }
    }

    for (i = 0 ; i < EF_HAL_ANALOG_TOTAL_CALL_BACK_CONV ; i++)
    {
        if (endConvCB[i] != NULL)
        {
            endConvCB[i](id, callBacks.aRead(id));
        }
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/*==================[end of file]============================================*/
