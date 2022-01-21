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
#include "appBoard.h"
#include "mma8451.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void appBoard_init(void)
{
    mma8451_ctrlReg4_t ctrlReg4;
    mma8451_ctrlReg5_t ctrlReg5;

    bsp_frdmkl46z_init();

    mma8451_init(efHal_dh_I2C0);

    ctrlReg4.INT_EN_DRDY = 1;
    ctrlReg4.INT_EN_FF_MT = 0;
    ctrlReg4.INT_EN_PULSE = 0;
    ctrlReg4.INT_EN_FIFO = 0;
    ctrlReg4.INT_EN_TRANS = 0;
    ctrlReg4.INT_EN_FIFO = 0;
    ctrlReg4.INT_EN_ASLP = 0;
    mma8451_setCtrlReg4(ctrlReg4);

    ctrlReg5.INT_CFG_DRDY = 1;
    ctrlReg5.INT_CFG_FF_MT = 0;
    ctrlReg5.INT_CFG_PULSE = 0;
    ctrlReg5.INT_CFG_LNDPRT = 0;
    ctrlReg5.INT_CFG_TRANS = 0;
    ctrlReg5.INT_CFG_FIFO = 0;
    ctrlReg5.INT_CFG_ASLP = 0;
    mma8451_setCtrlReg5(ctrlReg5);

    efHal_gpio_confInt(EF_HAL_INT1_ACCEL, EF_HAL_GPIO_INT_TYPE_FALLING_EDGE);
}

/*==================[end of file]============================================*/
