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
#include "efHal_uart.h"
#include "efHal_internal.h"

/*==================[macros and typedef]=====================================*/

#ifndef EF_HAL_UART_QUEUE_SEND_LENGTH
#define EF_HAL_UART_QUEUE_SEND_LENGTH 16
#endif

#ifndef EF_HAL_UART_QUEUE_RECV_LENGTH
#define EF_HAL_UART_QUEUE_RECV_LENGTH 16
#endif


typedef struct
{
    efHal_internal_dhD_t head;
    efHal_uart_callBacks_t cb;
    efHal_uart_conf_t conf;
    QueueHandle_t qSend;
    QueueHandle_t qRecv;
    bool txHasEnded;
    void* param;
}uart_dhD_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static uart_dhD_t dhD[EF_HAL_UART_TOTAL_DEVICES];

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
extern void efHal_uart_init(void)
{
    int i;

    for (i = 0 ; i < EF_HAL_UART_TOTAL_DEVICES ; i++)
    {
        dhD[i].head.mutex = NULL;
        dhD[i].param = NULL;
    }
}

extern void efHal_uart_conf(efHal_dh_t dh, efHal_uart_conf_t const *cfg)
{
    uart_dhD_t *dhD = dh;

    dhD->cb.conf(dhD->param, cfg);
}

extern uint32_t efHal_uart_getBaud(efHal_dh_t dh)
{
    uart_dhD_t *dhD = dh;

    return dhD->conf.baudrate;
}

extern uint32_t efHal_uart_getDataLength(efHal_dh_t dh)
{
    uart_dhD_t *dhD = dh;
    uint32_t ret = 1;   /* one start bit */

    if (dhD->conf.dataBits == EF_HAL_UART_DATA_BITS_8)
        ret += 8;
    else
        ret += 7;

    if (dhD->conf.parity != EF_HAL_UART_PARITY_NONE)
        ret += 1;

    if (dhD->conf.stopBits == EF_HAL_UART_STOP_BITS_2)
        ret += 2;
    else
        ret += 1;

    return ret;
}

extern int32_t efHal_uart_send(efHal_dh_t dh, void *pBuf, int32_t size, TickType_t blockTime)
{
    uart_dhD_t *dhD = dh;
    int32_t ret = 0;

    if (dhD->cb.sendBuffer != NULL)
    {
        ret = dhD->cb.sendBuffer(dhD->param, pBuf, size, blockTime);
    }
    else
    {
        while (ret < size)
        {
            if (xQueueSend(dhD->qSend, pBuf, blockTime) == pdTRUE)
            {
                if (dhD->txHasEnded)
                {
                    dhD->txHasEnded = false;
                    dhD->cb.dataReadyTx(dhD->param);
                }
                pBuf++;
                ret++;
            }
            else
            {
                break;
            }
        }
    }

    return ret;
}

extern int32_t efHal_uart_recv(efHal_dh_t dh, void *pBuf, int32_t size, TickType_t blockTime)
{
    uart_dhD_t *dhD = dh;

    int32_t ret = 0;

    while ((ret < size) &&
           (xQueueReceive( dhD->qRecv, pBuf, blockTime) == pdTRUE) )
    {
        ret++;
        pBuf++;
        blockTime = 0;
    }

    return ret;
}

extern efHal_dh_t efHal_internal_uart_deviceReg(efHal_uart_callBacks_t cb, void* param)
{
    uart_dhD_t *ret;

    taskENTER_CRITICAL();

    ret = efHal_internal_searchFreeSlot(&dhD[0].head, sizeof(uart_dhD_t), EF_HAL_UART_TOTAL_DEVICES);

    if (ret != NULL)
    {
        ret->head.mutex = xSemaphoreCreateMutex();
        ret->cb = cb;
        ret->param = param;
        ret->qSend = xQueueCreate(EF_HAL_UART_QUEUE_SEND_LENGTH, sizeof(uint8_t));
        ret->qRecv = xQueueCreate(EF_HAL_UART_QUEUE_RECV_LENGTH, sizeof(uint8_t));
    }

    taskEXIT_CRITICAL();

    return ret;
}

extern void efHal_internal_uart_putDataForRx(efHal_dh_t dh, void *pData)
{
    BaseType_t xHigherPriorityTaskWoken = false;
    uart_dhD_t *dhD = dh;

    xQueueSendFromISR( dhD->qRecv, pData, &xHigherPriorityTaskWoken );

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

extern bool efHal_internal_uart_getDataForTx(efHal_dh_t dh, void *pData)
{
    uint8_t data;
    uart_dhD_t *dhD = dh;
    BaseType_t xHigherPriorityTaskWoken = false;
    bool ret;

    if (xQueueReceiveFromISR( dhD->qSend,(void *) &data, &xHigherPriorityTaskWoken) == pdTRUE)
    {
        *(uint8_t*)pData = data;
        ret = true;
    }
    else
    {
        dhD->txHasEnded = true;
        ret = false;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    return ret;
}


/*==================[end of file]============================================*/
