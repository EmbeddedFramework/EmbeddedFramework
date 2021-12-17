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
#include "bsp_frdmkl46z_gpio.h"
#include "efHal_internal.h"

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "pin_mux.h"

/*==================[macros and typedef]=====================================*/
typedef struct
{
    PORT_Type *port;
    GPIO_Type *gpio;
    uint32_t pin;
    bool state;
}gpioStruct_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static const gpioStruct_t gpioStruct[] =
{
    {PORTE, GPIOE, 29, 1},     /* EF_HAL_GPIO_LED_RED */
    {PORTD, GPIOD, 5, 1},      /* EF_HAL_GPIO_LED_GREEN */
    {PORTC, GPIOC, 3, 0},      /* EF_HAL_GPIO_SW_1 */
    {PORTC, GPIOC, 12, 0},     /* EF_HAL_GPIO_SW_3 */
};

static const efHal_gpio_t gpioOut[] =
{
    EF_HAL_GPIO_LED_GREEN,
    EF_HAL_GPIO_LED_RED,
};

#define TOTAL_OUTPUTS   (sizeof(gpioOut) / sizeof(gpioOut[0]))

static const efHal_gpio_t gpioIn[] =
{
    EF_HAL_GPIO_SW_1,
    EF_HAL_GPIO_SW_3,
};

#define TOTAL_INPUTS   (sizeof(gpioIn) / sizeof(gpioIn[0]))

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void setPin(efHal_gpio_id_t id, bool state)
{
    GPIO_PortSet(gpioStruct[id].gpio, 1<<gpioStruct[id].pin);
}

static void togPin(efHal_gpio_id_t id)
{
    GPIO_PortToggle(gpioStruct[id].gpio, 1<<gpioStruct[id].pin);
}

static bool getPin(efHal_gpio_id_t id)
{
    return GPIO_ReadPinInput(gpioStruct[id].gpio, gpioStruct[id].pin);
}

static void confInt(efHal_gpio_id_t id, efHal_gpio_intType_t intType)
{
    port_interrupt_t portInt;

    switch (intType)
    {
        case EF_HAL_GPIO_INT_TYPE_DISABLE:
            portInt = kPORT_InterruptOrDMADisabled;
            break;

        case EF_HAL_GPIO_INT_TYPE_RISING_EDGE:
            portInt = kPORT_InterruptRisingEdge;
            break;

        case EF_HAL_GPIO_INT_TYPE_FALLING_EDGE:
            portInt = kPORT_InterruptFallingEdge;
            break;

        case EF_HAL_GPIO_INT_TYPE_BOTH_EDGE:
            portInt = kPORT_InterruptEitherEdge;
            break;

        default:
            /* TODO ASSERT */
            break;
    }

    PORT_SetPinInterruptConfig(gpioStruct[id].port, gpioStruct[id].pin, portInt);

    if (gpioStruct[id].port == PORTA)
        NVIC_EnableIRQ(PORTA_IRQn);
    else if (gpioStruct[id].port == PORTC)
        NVIC_EnableIRQ(PORTC_PORTD_IRQn);
    else if (gpioStruct[id].port == PORTD)
        NVIC_EnableIRQ(PORTC_PORTD_IRQn);
    else
    {
        /* TODO ASSERT */
    }
}

static void confDir(efHal_gpio_id_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull, bool state)
{
    gpio_pin_config_t gpio_pin_config;

    gpio_pin_config.outputLogic = state;
    gpio_pin_config.pinDirection = (dir==EF_HAL_GPIO_INPUT)? kGPIO_DigitalInput : kGPIO_DigitalOutput;

    port_pin_config_t port_pin_config;

    switch (pull)
    {
        case EF_HAL_GPIO_PULL_DISABLE:
            port_pin_config.pullSelect = kPORT_PullDisable;
            break;

        case EF_HAL_GPIO_PULL_UP:
            port_pin_config.pullSelect = kPORT_PullUp;
            break;

        case EF_HAL_GPIO_PULL_DOWN:
            port_pin_config.pullSelect = kPORT_PullDown;
            break;
    }

    port_pin_config.slewRate = kPORT_SlowSlewRate;
    port_pin_config.passiveFilterEnable = kPORT_PassiveFilterDisable;
    port_pin_config.driveStrength = kPORT_LowDriveStrength;
    port_pin_config.mux = kPORT_MuxAsGpio;

    PORT_SetPinConfig(gpioStruct[id].port, gpioStruct[id].pin, &port_pin_config);
    GPIO_PinInit(gpioStruct[id].gpio, gpioStruct[id].pin, &gpio_pin_config);
}

/*==================[external functions definition]==========================*/
extern void bsp_frdmkl46z_gpio_init(void)
{
    efHal_gpio_callBacks_t cb;
    int i;

    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);

    cb.setPin = setPin;
    cb.togPin = togPin;
    cb.getPin = getPin;
    cb.confInt = confInt;
    cb.confPin = confDir;

    efHal_internal_gpio_setCallBacks(cb);

    for (i = 0 ; i < TOTAL_OUTPUTS ; i++)
        confDir(gpioOut[i], EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, gpioStruct[gpioOut[i]].state);

    for (i = 0 ; i < TOTAL_INPUTS; i++)
        confDir(gpioIn[i], EF_HAL_GPIO_INPUT, EF_HAL_GPIO_PULL_DISABLE, 0);
}

/*==================[end of file]============================================*/
