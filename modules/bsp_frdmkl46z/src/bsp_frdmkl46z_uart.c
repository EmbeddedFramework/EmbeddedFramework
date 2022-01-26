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
#include "bsp_frdmkl46z_uart.h"
#include "efHal_internal.h"
#include "fsl_uart.h"
#include "fsl_port.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
efHal_dh_t efHal_dh_UART1;

/*==================[internal functions definition]==========================*/

static void uart_config(void)
{
    uart_config_t config;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(UART1, &config, CLOCK_GetFreq(BUS_CLK));

    /* Configura los pines RX y TX de la UART1 */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);

    UART_EnableInterrupts(UART1, kUART_RxDataRegFullInterruptEnable);
    UART_EnableInterrupts(UART1, kUART_TxDataRegEmptyInterruptEnable);
    UART_EnableInterrupts(UART1, kUART_TransmissionCompleteInterruptEnable);

    NVIC_EnableIRQ(UART1_IRQn);
}

/*==================[external functions definition]==========================*/
extern void bsp_frdmkl46z_uart_init(void)
{
    efHal_uart_callBacks_t cb;

    uart_config();

    efHal_dh_UART1 = efHal_internal_uart_deviceReg(cb, UART1);
}

void UART1_IRQHandler(void)
{
    uint8_t data;

    if ( (kUART_RxDataRegFullFlag)            & UART_GetStatusFlags(UART1) &&
         (kUART_RxDataRegFullInterruptEnable) & UART_GetEnabledInterrupts(UART1) )
    {
        data = UART_ReadByte(UART1);
        efHal_internal_uart_putDataForRx(efHal_dh_UART1, &data);
    }

    if ( (kUART_TxDataRegEmptyFlag)            & UART_GetStatusFlags(UART1) &&
         (kUART_TxDataRegEmptyInterruptEnable) & UART_GetEnabledInterrupts(UART1) )
    {
        while ((kUART_TxDataRegEmptyFlag) & UART_GetStatusFlags(UART1))
        {
            if (efHal_internal_uart_getDataForTx(efHal_dh_UART1, &data))
                UART_WriteByte(UART1, data);
            else
                UART_DisableInterrupts(UART1, kUART_TxDataRegEmptyInterruptEnable);
        }
    }

    if ( (kUART_TransmissionCompleteFlag)            & UART_GetStatusFlags(UART1) &&
         (kUART_TransmissionCompleteInterruptEnable) & UART_GetEnabledInterrupts(UART1) )
    {
        UART_DisableInterrupts(UART1, kUART_TransmissionCompleteInterruptEnable);
        UART_ClearStatusFlags(UART1, kUART_TransmissionCompleteFlag);
    }
}

/*==================[end of file]============================================*/
