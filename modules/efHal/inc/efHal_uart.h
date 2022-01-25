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
#ifndef EF_HAL_UART_H_
#define EF_HAL_UART_H_

/*==================[inclusions]=============================================*/
#include "efHal.h"
#include "stddef.h"
#include "FreeRTOS.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

typedef enum
{
   EF_HAL_UART_DATA_BITS_7 = 0,
   EF_HAL_UART_DATA_BITS_8,
}efHal_uart_dataBits_t;

typedef enum
{
   EF_HAL_UART_PARITY_NONE = 0,
   EF_HAL_UART_PARITY_EVEN,
   EF_HAL_UART_PARITY_ODD,
}efHal_uart_parity_t;

typedef enum
{
   EF_HAL_UART_STOP_BITS_1 = 0,
   EF_HAL_UART_STOP_BITS_2,
}efHal_uart_stopBits_t;

typedef struct
{
   uint32_t baudrate;
   efHal_uart_dataBits_t dataBits;
   efHal_uart_parity_t parity;
   efHal_uart_stopBits_t stopBits;
}efHal_uart_conf_t;

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern void efHal_uart_init(void);
extern void efHal_uart_conf(efHal_dh_t dh, efHal_uart_conf_t const *cfg);
extern uint32_t efHal_uart_getBaud(efHal_dh_t dh);
extern uint32_t efHal_uart_getDataLength(efHal_dh_t dh);
extern int32_t efHal_uart_send(efHal_dh_t dh, void *pBuf, int32_t size, TickType_t blockTime);
extern int32_t efHal_uart_recv(efHal_dh_t dh, void *pBuf, int32_t size, TickType_t blockTime);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* EF_HAL_UART_H_ */
