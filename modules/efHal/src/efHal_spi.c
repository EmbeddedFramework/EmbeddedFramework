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
#include "efHal_spi.h"
#include "efHal_internal.h"

/*==================[macros and typedef]=====================================*/
typedef struct
{
    efHal_internal_dhD_t head;
    efHal_spi_callBacks_t cb;
    void* param;
}spi_dhD_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static spi_dhD_t dhD[EF_HAL_SPI_TOTAL_DEVICES];


/*==================[external functions definition]==========================*/

extern void efHal_spi_init(void)
{
    int i;

    for (i = 0 ; i < EF_HAL_SPI_TOTAL_DEVICES ; i++)
    {
        dhD[i].head.mutex = NULL;
        dhD[i].param = NULL;
    }
}

extern void efHal_spi_config(efHal_dh_t dh, int32_t clockFrec, efHal_spi_mode_t mode)
{
    spi_dhD_t *p_dhD = dh;

    xSemaphoreTake(p_dhD->head.mutex, portMAX_DELAY);
    p_dhD->cb.conf(p_dhD->param, clockFrec, mode);
    xSemaphoreGive(p_dhD->head.mutex);
}

extern void efHal_spi_transfer(efHal_dh_t dh, void *pTx, void *pRx, size_t length)
{
    spi_dhD_t *p_dhD = dh;

    xSemaphoreTake(p_dhD->head.mutex, portMAX_DELAY);

    p_dhD->head.taskHadle = xTaskGetCurrentTaskHandle();
    xTaskNotifyStateClear(p_dhD->head.taskHadle);
    p_dhD->cb.transfer(p_dhD->param, pTx, pRx, length);
    xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
    xSemaphoreGive(p_dhD->head.mutex);
}

extern void efHal_internal_spi_endOfTransfer(efHal_internal_dhD_t *p_dhD)
{
    xTaskNotify(p_dhD->taskHadle, 0, eNoAction);
}

extern efHal_dh_t efHal_internal_spi_deviceReg(efHal_spi_callBacks_t cb_dev, void* param)
{
    spi_dhD_t *ret;

    taskENTER_CRITICAL();

    ret = efHal_internal_searchFreeSlot(&dhD[0].head, sizeof(spi_dhD_t), EF_HAL_SPI_TOTAL_DEVICES);

    if (ret != NULL)
    {
        ret->head.mutex = xSemaphoreCreateMutex();
        ret->cb = cb_dev;
        ret->param = param;
    }

    taskEXIT_CRITICAL();

    return ret;
}

/*==================[end of file]============================================*/
