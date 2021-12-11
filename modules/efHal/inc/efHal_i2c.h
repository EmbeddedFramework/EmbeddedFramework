/*
###############################################################################
#
# Copyright 2021, Gustavo Muro
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
#ifndef EF_HAL_I2C_H_
#define EF_HAL_I2C_H_

/*==================[inclusions]=============================================*/
#include "efHal.h"
#include "stddef.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

typedef enum
{
    EF_HAL_I2C_EC_NO_ERROR = 0,
    EF_HAL_I2C_EC_INVALID_HANDLER,
    EF_HAL_I2C_EC_INVALID_PARAMS,
    EF_HAL_I2C_EC_TRANSFER_UNSUPPORTED,
    EF_HAL_I2C_EC_NAK,
    EF_HAL_I2C_EC_UNKNOW,
}efHal_i2c_ec_t;

typedef uint8_t efHal_i2c_devAdd_t;

typedef efHal_i2c_ec_t (*efHal_i2c_deviceTransfer_t)(void* param, efHal_i2c_devAdd_t da, void *pTx, size_t sTx, void *pRx, size_t sRx);

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern void efHal_i2c_init(void);

extern efHal_dh_t efHal_i2c_deviceReg(efHal_i2c_deviceTransfer_t cb_devTra, void* param);

extern efHal_i2c_ec_t efHal_i2c_transfer(efHal_dh_t dh, efHal_i2c_devAdd_t da, void *pTx, size_t sTx, void *pRx, size_t sRx);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* EF_HAL_I2C_H_ */
