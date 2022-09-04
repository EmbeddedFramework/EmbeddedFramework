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
#include "efHal_pwm.h"
#include "efHal_internal.h"

/*==================[macros and typedef]=====================================*/

typedef struct
{
    efHal_pwm_callBackInt_t cbInt;
    efHal_pwm_id_t pwmId;
}cb_pwm_t;

typedef struct
{
    TaskHandle_t taskHandle;
    efHal_pwm_id_t pwmId;
}taskHandle_pwm_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static cb_pwm_t cb_pwm[EF_HAL_PWM_TOTAL_CALL_BACK];
static taskHandle_pwm_t taskHandle_pwm[EF_HAL_PWM_TOTAL_WAIT_FOR_INT];

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void efHal_pwm_init(void)
{
    int i;
    for (i = 0 ; i < EF_HAL_PWM_TOTAL_CALL_BACK ; i++)
    {
        cb_pwm[i].cbInt = NULL;
        cb_pwm[i].pwmId = 0;
    }
}

extern bool efHal_pwm_waitForInt(efHal_pwm_id_t id, TickType_t xBlockTime)
{
    int i;
    bool ret = false;

    for (i = 0 ; i < EF_HAL_PWM_TOTAL_WAIT_FOR_INT ; i++)
    {
        if (taskHandle_pwm[i].taskHandle == NULL)
        {
            taskHandle_pwm[i].pwmId = id;
            taskHandle_pwm[i].taskHandle = xTaskGetCurrentTaskHandle();
            xTaskNotifyStateClear(taskHandle_pwm[i].taskHandle);
            break;
        }
    }

    /* check if no free slot error */
    if (i >= EF_HAL_PWM_TOTAL_WAIT_FOR_INT)
    {
        /* TODO ASSERT */
    }

    if (ulTaskNotifyTake(pdTRUE, xBlockTime))
        ret = true;

    taskHandle_pwm[i].taskHandle = NULL;

    return ret;
}

extern void efHal_pwm_setCallBackInt(efHal_pwm_id_t id, efHal_pwm_callBackInt_t cb)
{
    int i;
    for (i = 0 ; i < EF_HAL_PWM_TOTAL_CALL_BACK ; i++)
    {
        if (cb_pwm[i].cbInt == NULL)
        {
            cb_pwm[i].pwmId = id;
            cb_pwm[i].cbInt = cb;
            break;
        }
    }

    /* check if no free slot error */
    if (i >= EF_HAL_PWM_TOTAL_CALL_BACK)
    {
        /* TODO ASSERT */
    }
}

extern void efHal_internal_pwm_InterruptRoutine(efHal_pwm_id_t id)
{
    int i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    for (i = 0 ; i < EF_HAL_PWM_TOTAL_CALL_BACK ; i++)
    {
        if (cb_pwm[i].pwmId == id)
        {
            if (cb_pwm[i].cbInt != NULL)
            {
                cb_pwm[i].cbInt(id);
            }
            else
            {
                /* TODO ASSERT */
            }

            break;
        }
    }

    for (i = 0 ; i < EF_HAL_PWM_TOTAL_WAIT_FOR_INT ; i++)
    {
        if (taskHandle_pwm[i].pwmId == id)
        {
            if (taskHandle_pwm[i].taskHandle != NULL)
            {
                vTaskNotifyGiveFromISR(taskHandle_pwm[i].taskHandle, &xHigherPriorityTaskWoken);
            }
            else
            {
                /* TODO ASSERT */
            }

            break;
        }
    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/*==================[end of file]============================================*/
