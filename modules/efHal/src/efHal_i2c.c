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

/*==================[inclusions]=============================================*/
#include "efHal_i2c.h"
#include "efHal_internal.h"

/*==================[macros and typedef]=====================================*/
typedef struct
{
    efHal_internal_dhD_t head;
    efHal_i2c_deviceTransfer_t cb;
    void* param;
}i2c_dhD_t;

#define EF_HAL_I2C_TOTAL_DEVICES    2

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static i2c_dhD_t dhD[EF_HAL_I2C_TOTAL_DEVICES];


/*==================[external functions definition]==========================*/

extern void efHal_i2c_init(void)
{
    int i;

    for (i = 0 ; i < EF_HAL_I2C_TOTAL_DEVICES ; i++)
    {
        dhD[i].head.mutex = NULL;
        dhD[i].cb = NULL;
        dhD[i].param = NULL;
    }
}

extern efHal_dh efHal_i2c_deviceReg(efHal_i2c_deviceTransfer_t cb_devTra, void* param)
{
    i2c_dhD_t *ret;

    ret = efHal_internal_searchFreeSlot(&dhD[0].head, sizeof(i2c_dhD_t), EF_HAL_I2C_TOTAL_DEVICES);

    if (ret != NULL)
    {
        ret->cb = cb_devTra;
        ret->param = param;
    }

    return ret;
}

extern efHal_i2c_ec_t efHal_i2c_transfer(efHal_dh dh, void *pTx, size_t sTx, void *pRx, size_t sRx)
{
    efHal_i2c_ec_t ret;

    i2c_dhD_t *p_dhD = dh;

    if (p_dhD == NULL)
        ret = EF_HAL_I2C_EC_INVALID_HANDLER;
    else
    {
        xSemaphoreTake(p_dhD->head.mutex, portMAX_DELAY);
        ret = p_dhD->cb(p_dhD->param, pTx, sTx, pRx, sRx);
        xSemaphoreGive(p_dhD->head.mutex);
    }

    return ret;
}

/*==================[end of file]============================================*/
