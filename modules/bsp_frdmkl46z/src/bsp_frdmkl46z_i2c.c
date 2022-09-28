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
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "efHal_internal.h"

/*==================[macros and typedef]=====================================*/

#define I2C_BAUDRATE            100000U

#define I2C_RELEASE_SDA_PORT PORTE
#define I2C_RELEASE_SCL_PORT PORTE
#define I2C_RELEASE_SDA_GPIO GPIOE
#define I2C_RELEASE_SDA_PIN 25U
#define I2C_RELEASE_SCL_GPIO GPIOE
#define I2C_RELEASE_SCL_PIN 24U
#define I2C_RELEASE_BUS_COUNT 100U

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

    /* there aren't reception */
    if (sRx == 0 || pRx == NULL)
    {
        if (pTx == NULL || sTx == 0)
        {
            efErrorHdl_error(EF_ERROR_HDL_INVALID_PARAMETER, "pTx == NULL || sTx == 0");
            ret = EF_HAL_I2C_EC_INVALID_PARAMS;
        }
        else
        {
            masterXfer.direction = kI2C_Write;
            masterXfer.subaddress = 0;
            masterXfer.subaddressSize = 0;
            masterXfer.data = pTx;
            masterXfer.dataSize = sTx;
        }
    }
    else
    {
        masterXfer.direction = kI2C_Read;

        if (sTx > sizeof(masterXfer.subaddress))
        {
            efErrorHdl_error(EF_HAL_I2C_EC_TRANSFER_UNSUPPORTED, "sTx > sizeof(masterXfer.subaddress)");
            ret = EF_HAL_I2C_EC_TRANSFER_UNSUPPORTED;
        }
        else if (pTx != NULL && sTx > 0)
        {
            int i;
            masterXfer.subaddress = 0;

            for (i = 0 ; i < sTx ; i++)
                masterXfer.subaddress |= ((uint8_t*)pTx)[sTx-i-1] << (8*i);

            masterXfer.subaddressSize = sTx;
            masterXfer.data = pRx;
            masterXfer.dataSize = sRx;
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
    efHal_i2c_ec_t ec;

    switch (status)
    {
        case kStatus_Success:
            ec = EF_HAL_I2C_EC_NO_ERROR;
            break;

        case kStatus_I2C_Nak:
        case kStatus_I2C_Addr_Nak:
            ec = EF_HAL_I2C_EC_NAK;
            break;

        default:
            ec = EF_HAL_I2C_EC_UNKNOW;
            break;
    }

    efHal_internal_i2c_endOfTransfer(userData, ec);
}

static void i2c_configPins(void)
{
    const port_pin_config_t porte24_pin31_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SCL */
                                                    kPORT_MuxAlt5};
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTE24 (pin 31) is configured as I2C0_SCL */
    PORT_SetPinConfig(PORTE, 24U, &porte24_pin31_config);

    const port_pin_config_t porte25_pin32_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SDA */
                                                    kPORT_MuxAlt5};
    /* PORTE25 (pin 32) is configured as I2C0_SDA */
    PORT_SetPinConfig(PORTE, 25U, &porte25_pin32_config);
}

static void i2c_config(void)
{
    i2c_master_config_t masterConfig;
    uint32_t sourceClock;
    /*
     * masterConfig.baudRate_Bps = 100000U;
     * masterConfig.enableStopHold = false;
     * masterConfig.glitchFilterWidth = 0U;
     * masterConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);

    masterConfig.baudRate_Bps = I2C_BAUDRATE;

    sourceClock = CLOCK_GetFreq(I2C0_CLK_SRC);

    I2C_MasterInit(I2C0, &masterConfig, sourceClock);
}

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
    {
        __NOP();
    }
}

void i2c_releaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux = kPORT_MuxAsGpio;

    pin_config.pinDirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    CLOCK_EnableClock(kCLOCK_PortE);
    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
    PORT_SetPinConfig(I2C_RELEASE_SDA_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);

    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA high */
    for (i = 0; i < 9; i++)
    {
        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
        i2c_release_bus_delay();

        GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
    i2c_release_bus_delay();

    GPIO_PinWrite(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
    i2c_release_bus_delay();
}

/*==================[external functions definition]==========================*/
extern void bsp_frdmkl46z_i2c_init(void)
{
    i2c_releaseBus();
    i2c_configPins();
    i2c_config();

    efHal_dh_I2C0 = efHal_internal_i2c_deviceReg(bsp_frdmkl46z_i2c_deviceTransfer, I2C0);

    I2C_MasterTransferCreateHandle(I2C0, &m_handle, i2c_master_callback, efHal_dh_I2C0);
}


/*==================[end of file]============================================*/
