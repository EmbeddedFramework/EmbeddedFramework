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
#include "mma8451.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "efHal_i2c.h"
#include "stdbool.h"

/*==================[macros and typedef]=====================================*/

#define MMA8451_I2C_ADDRESS     (0x1d)

#define STATUS_ADDRESS      0X00
#define OUT_ADDRESS         0X01
#define CTRL_REG1_ADDRESS   0X2A
#define CTRL_REG4_ADDRESS   0X2D
#define CTRL_REG5_ADDRESS   0X2E
#define INT_SOURCE_ADDRESS  0X0C

#define ACC_INT_COUNT_LENGTH    6

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static efHal_dh_t i2c_dh;
static SemaphoreHandle_t xMutexAcc;

static mma8451_ctrlReg1_t reg1;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void mma8451_write_reg(uint8_t addr, uint8_t data)
{
    uint8_t txBuf[] =
    {
        addr,
        data,
    };

    efHal_i2c_transfer(i2c_dh, MMA8451_I2C_ADDRESS, txBuf, sizeof(txBuf), NULL, 0);
}

static void mma8451_read_regs(uint8_t addr, uint8_t *buf, size_t length)
{
    efHal_i2c_transfer(i2c_dh, MMA8451_I2C_ADDRESS, &addr, sizeof(addr), buf, length);
}

static void writeRegWithActiveLow(uint8_t addr, uint8_t data)
{
    bool prevActive = 0;
    uint8_t *pTmp = (uint8_t*)&reg1;

    if (reg1.ACTIVE)
    {
        prevActive = 1;
        reg1.ACTIVE = 0;
        mma8451_write_reg(CTRL_REG1_ADDRESS, *pTmp);
    }

    mma8451_write_reg(addr, data);

    if (prevActive)
    {
        reg1.ACTIVE = 1;
        mma8451_write_reg(CTRL_REG1_ADDRESS, *pTmp);
    }
}

/*==================[external functions definition]==========================*/
void mma8451_init(efHal_dh_t dh)
{
    i2c_dh = dh;

    xMutexAcc = xSemaphoreCreateMutex();

    reg1.ACTIVE = 1;
    reg1.F_READ = 0;
    reg1.LNOISE = 0;
    reg1.DR = MMA8451_DR_12p5hz;
    reg1.ASLP_RATE = MMA8451_ASLP_6p25hz;

    mma8451_setCtrlReg1(reg1);
}

void mma8451_setDataRate(mma8451_DR_t rate)
{
    reg1.DR = rate;

    mma8451_setCtrlReg1(reg1);
}

extern void mma8451_setCtrlReg1(mma8451_ctrlReg1_t reg)
{
    uint8_t *pTmp = (uint8_t*)&reg;

    xSemaphoreTake(xMutexAcc, portMAX_DELAY);
    reg1 = reg;
    writeRegWithActiveLow(CTRL_REG1_ADDRESS, *pTmp);
    xSemaphoreGive(xMutexAcc);
}

extern void mma8451_setCtrlReg4(mma8451_ctrlReg4_t reg4)
{
    uint8_t *pTmp = (uint8_t*)&reg4;

    xSemaphoreTake(xMutexAcc, portMAX_DELAY);
    mma8451_write_reg(CTRL_REG4_ADDRESS, *pTmp);
    xSemaphoreGive(xMutexAcc);
}

extern void mma8451_setCtrlReg5(mma8451_ctrlReg5_t reg5)
{
    uint8_t *pTmp = (uint8_t*)&reg5;

    xSemaphoreTake(xMutexAcc, portMAX_DELAY);
    writeRegWithActiveLow(CTRL_REG5_ADDRESS, *pTmp);
    xSemaphoreGive(xMutexAcc);
}

extern mma8451_accIntCount_t mma8451_getAccIntCount(void)
{
    mma8451_accIntCount_t ret;
    uint8_t buf[ACC_INT_COUNT_LENGTH];

    xSemaphoreTake(xMutexAcc, portMAX_DELAY);
    mma8451_read_regs(OUT_ADDRESS, buf, ACC_INT_COUNT_LENGTH);
    xSemaphoreGive(xMutexAcc);

    ret.accX = ((int16_t)((int16_t)buf[0]<<8 | buf[1])) >> 2;
    ret.accY = ((int16_t)((int16_t)buf[2]<<8 | buf[3])) >> 2;
    ret.accZ = ((int16_t)((int16_t)buf[4]<<8 | buf[5])) >> 2;

    return ret;
}

/*==================[end of file]============================================*/
