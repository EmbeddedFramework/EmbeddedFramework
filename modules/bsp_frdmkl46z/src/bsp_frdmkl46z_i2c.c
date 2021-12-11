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
#include "bsp_frdmkl46z_i2c.h"
#include "fsl_i2c.h"
#include "efHal_internal.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/
static i2c_master_handle_t m_handle;

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/
efHal_dh_t efHal_dh_I2C0;

/*==================[internal functions definition]==========================*/

static efHal_i2c_ec_t bsp_frdmkl46z_i2c_deviceTransfer(void* param, efHal_i2c_devAdd_t da, void *pTx, size_t sTx, void *pRx, size_t sRx)
{
    efHal_i2c_ec_t ret = EF_HAL_I2C_EC_NO_ERROR;
    i2c_master_transfer_t masterXfer;

    memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress = da;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    if (pRx == NULL || sRx == 0)
        masterXfer.direction = kI2C_Read;
    else
    {
        masterXfer.direction = kI2C_Write;
        if (pTx == NULL || sTx == 0)
            ret = EF_HAL_I2C_EC_INVALID_PARAMS;
    }

    if (ret == EF_HAL_I2C_EC_NO_ERROR)
    {
        /* transmission equal or less than 4 byte */
        if (sTx <= sizeof(masterXfer.subaddress))
        {
            int i;

            masterXfer.subaddress = 0;

            for (i = 0 ; i < sTx ; i++)
                masterXfer.subaddress |= ((uint8_t*)pTx)[i] << (8*i);

            masterXfer.subaddressSize = sTx;
            masterXfer.data = pRx;
            masterXfer.dataSize = sRx;
        }
        else
        {   /* transmission greater than 4 bytes */

            /* if there aren't reception, use only data for transfer */
            if (pRx == NULL || sRx == 0)
            {
                masterXfer.subaddress = 0;
                masterXfer.subaddressSize = 0;
                masterXfer.data = pTx;
                masterXfer.dataSize = sTx;
            }
            /* TODO: implements transfer in two part */
            else
            {
                ret = EF_HAL_I2C_EC_TRANSFER_UNSUPPORTED;
            }
        }
    }

    if (ret == EF_HAL_I2C_EC_NO_ERROR)
    {
        I2C_MasterTransferNonBlocking(param, &m_handle, &masterXfer);
    }

    return ret;
}

static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    switch (status)
    {
        case kStatus_Success:
            efHal_internal_i2c_endOfTransfer(userData, EF_HAL_I2C_EC_NO_ERROR);
            break;

        case kStatus_I2C_Nak:
        case kStatus_I2C_Addr_Nak:
            efHal_internal_i2c_endOfTransfer(userData, EF_HAL_I2C_EC_NO_ERROR);
            break;
    }
}

/*==================[external functions definition]==========================*/
extern void bsp_frdmkl46z_i2c_init(void)
{
    efHal_dh_I2C0 = efHal_i2c_deviceReg(bsp_frdmkl46z_i2c_deviceTransfer, I2C0);

    I2C_MasterTransferCreateHandle(I2C0, &m_handle, i2c_master_callback, efHal_dh_I2C0);
}


/*==================[end of file]============================================*/
