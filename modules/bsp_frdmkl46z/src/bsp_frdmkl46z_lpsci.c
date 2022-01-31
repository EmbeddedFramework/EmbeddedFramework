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
#include "bsp_frdmkl46z_lpsci.h"
#include "efHal_internal.h"
#include "fsl_lpsci.h"
#include "fsl_port.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
efHal_dh_t efHal_dh_UART0;

/*==================[internal functions definition]==========================*/

static void lpsci_init(void)
{
    lpsci_config_t config;

    CLOCK_SetLpsci0Clock(0x1U);

    /* PORTA1 (pin 35) is configured as UART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);
    /* PORTA2 (pin 36) is configured as UART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    /*
     * config.parityMode = kLPSCI_ParityDisabled;
     * config.stopBitCount = kLPSCI_OneStopBit;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPSCI_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200;
    config.parityMode = kLPSCI_ParityDisabled;
    config.stopBitCount = kLPSCI_OneStopBit;
    config.enableTx = true;
    config.enableRx = true;

    LPSCI_Init(UART0, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));

    LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);
    LPSCI_EnableInterrupts(UART0, kLPSCI_TxDataRegEmptyInterruptEnable);
    LPSCI_EnableInterrupts(UART0, kLPSCI_TransmissionCompleteInterruptEnable);

    NVIC_EnableIRQ(UART0_IRQn);
}

static void confCB(void *param, efHal_uart_conf_t const *cfg)
{
    lpsci_config_t config;

    LPSCI_GetDefaultConfig(&config);
    config.baudRate_Bps = cfg->baudrate;

    switch (cfg->parity)
    {
        case EF_HAL_UART_PARITY_NONE:
            config.parityMode = kLPSCI_ParityDisabled;
            break;

        case EF_HAL_UART_PARITY_EVEN:
            config.parityMode = kLPSCI_ParityEven;
            break;

        case EF_HAL_UART_PARITY_ODD:
            config.parityMode = kLPSCI_ParityOdd;
            break;
    }

    if (cfg->stopBits == EF_HAL_UART_STOP_BITS_2)
        config.stopBitCount = kLPSCI_TwoStopBit;
    else
        config.stopBitCount = kLPSCI_OneStopBit;

    if (cfg->dataBits == EF_HAL_UART_DATA_BITS_7)
    {
        /* This uC doesn't support 7 data bits */
        /* TODO: evaluate to use 8 data bits and add parity bit by software */
    }

    config.enableTx = true;
    config.enableRx = true;

    LPSCI_Init(UART0, &config, CLOCK_GetFreq(BUS_CLK));
}

static void dataReadyTx(void *param)
{
    LPSCI_EnableInterrupts(param, kLPSCI_TxDataRegEmptyInterruptEnable);
}


/*==================[external functions definition]==========================*/
extern void bsp_frdmkl46z_lpsci_init(void)
{
    efHal_uart_callBacks_t cb;

    efHal_dh_UART0 = efHal_internal_uart_deviceReg(cb, UART0);

    cb.conf = confCB;
    cb.dataReadyTx = dataReadyTx;

    lpsci_init();
}

void UART0_IRQHandler(void)
{
    uint8_t data;

    if ( (kLPSCI_RxDataRegFullFlag)            & LPSCI_GetStatusFlags(UART0) &&
         (kLPSCI_RxDataRegFullInterruptEnable) & LPSCI_GetEnabledInterrupts(UART0) )
    {
        data = LPSCI_ReadByte(UART0);
        efHal_internal_uart_putDataForRx(efHal_dh_UART0, &data);
    }

    if ( (kLPSCI_TxDataRegEmptyFlag)            & LPSCI_GetStatusFlags(UART0) &&
         (kLPSCI_TxDataRegEmptyInterruptEnable) & LPSCI_GetEnabledInterrupts(UART0) )
    {
        while ((kLPSCI_TxDataRegEmptyFlag) & LPSCI_GetStatusFlags(UART0))
        {
            if (efHal_internal_uart_getDataForTx(efHal_dh_UART0, &data))
                LPSCI_WriteByte(UART0, data);
            else
            {
                LPSCI_DisableInterrupts(UART0, kLPSCI_TxDataRegEmptyInterruptEnable);
                break;
            }
        }
    }

    if ( (kLPSCI_TransmissionCompleteFlag)            & LPSCI_GetStatusFlags(UART0) &&
         (kLPSCI_TransmissionCompleteInterruptEnable) & LPSCI_GetEnabledInterrupts(UART0) )
    {
        LPSCI_DisableInterrupts(UART0, kLPSCI_TransmissionCompleteInterruptEnable);
        LPSCI_ClearStatusFlags(UART0, kLPSCI_TransmissionCompleteFlag);
    }
}

/*==================[end of file]============================================*/
