/*
###############################################################################
#
# Copyright 2023, Gustavo Garcia
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

#include "bsp_frdmkl46z_spi.h"
#include "efHal_internal.h"

#include "fsl_spi.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "pin_mux.h"

/*==================[macros and typedef]=====================================*/

#define SPI_MASTER_SOURCE_CLOCK kCLOCK_BusClk
#define SPI_MASTER_CLK_FREQ     CLOCK_GetFreq(kCLOCK_BusClk)

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static spi_master_handle_t handle;

/*==================[external data definition]===============================*/

efHal_dh_t efHal_dh_SPI0;

/*==================[internal functions definition]==========================*/

static void spi_ConfCB(void *param, int32_t clockFrec, efHal_spi_mode_t mode)
{

	const port_pin_config_t port_spi_config = {
		/* Internal pull-up resistor is disabled */
		.pullSelect = kPORT_PullDisable,
		/* Fast slew rate is configured */
		.slewRate = kPORT_FastSlewRate,
		/* Passive filter is disabled */
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		/* Low drive strength is configured */
		.driveStrength = kPORT_LowDriveStrength,
		/* Pin is configured as SPI0_x */
		.mux = kPORT_MuxAlt2,
	};

	PORT_SetPinConfig(PORTE, 16, &port_spi_config); //SPI0_SS
	PORT_SetPinConfig(PORTE, 17, &port_spi_config); //SPI0_SCK
	PORT_SetPinConfig(PORTE, 18, &port_spi_config); //SPI0_MOSI
	PORT_SetPinConfig(PORTE, 19, &port_spi_config); //SPI0_MISO

	CLOCK_EnableClock(kCLOCK_Spi0);

	spi_master_config_t userConfig;

	SPI_MasterGetDefaultConfig(&userConfig);

	/*
	userConfig.enableMaster         = true;
	userConfig.enableStopInWaitMode = false;
	userConfig.polarity             = kSPI_ClockPolarityActiveHigh;
	userConfig.phase                = kSPI_ClockPhaseFirstEdge;
	userConfig.direction            = kSPI_MsbFirst;
	userConfig.dataMode 			 = kSPI_8BitMode;
	userConfig.txWatermark 		 = kSPI_TxFifoOneHalfEmpty;
	userConfig.rxWatermark 		 = kSPI_RxFifoOneHalfFull;
	userConfig.pinMode      		 = kSPI_PinModeNormal;
	userConfig.outputMode   		 = kSPI_SlaveSelectAutomaticOutput;
	userConfig.baudRate_Bps 		 = 500000U;
	*/
	userConfig.baudRate_Bps         = clockFrec;

	switch (mode){
		case EF_HAL_SPI_CPOL_0_CPHA_O:
			userConfig.polarity = kSPI_ClockPolarityActiveHigh;
			userConfig.phase = kSPI_ClockPhaseFirstEdge;
			break;

		case EF_HAL_SPI_CPOL_0_CPHA_1:
			userConfig.polarity = kSPI_ClockPolarityActiveHigh;
			userConfig.phase = kSPI_ClockPhaseSecondEdge;
			break;

		case EF_HAL_SPI_CPOL_1_CPHA_O:
			userConfig.polarity = kSPI_ClockPolarityActiveLow;
			userConfig.phase = kSPI_ClockPhaseFirstEdge;
			break;

		case EF_HAL_SPI_CPOL_1_CPHA_1:
			userConfig.polarity = kSPI_ClockPolarityActiveLow;
			userConfig.phase = kSPI_ClockPhaseSecondEdge;
			break;
	}

	SPI_MasterInit(param, &userConfig, SPI_MASTER_CLK_FREQ);
}

static void SPI_MasterInterruptCallback(SPI_Type *base, spi_master_handle_t *masterHandle,status_t status, void *userData){
    efHal_internal_spi_endOfTransfer(userData);
}

static void spi_TransferCB(void* param, void *pTx, void *pRx, size_t length)
{
    spi_transfer_t tempXfer = {0};

    tempXfer.txData = pTx;
    tempXfer.rxData = pRx;
    tempXfer.dataSize = length;

    SPI_MasterTransferNonBlocking(param, &handle, &tempXfer);
}

/*==================[external functions definition]==========================*/

extern void bsp_frdmkl46z_spi_init(void)
{
    efHal_spi_callBacks_t cb;

    cb.conf = spi_ConfCB;
    cb.transfer = spi_TransferCB;

    efHal_dh_SPI0 = efHal_internal_spi_deviceReg(cb, SPI0);
    SPI_MasterTransferCreateHandle(SPI0, &handle, SPI_MasterInterruptCallback, efHal_dh_SPI0);
}

/*==================[end of file]============================================*/




