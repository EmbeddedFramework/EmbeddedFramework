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
}gpioStruct_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static const gpioStruct_t gpioStruct[] =
{
    {PORTE, GPIOE, 29},     /* EF_HAL_GPIO_LED_RED */
    {PORTD, GPIOD, 5},      /* EF_HAL_GPIO_LED_GREEN */
    {PORTC, GPIOC, 3},      /* EF_HAL_GPIO_SW_1 */
    {PORTC, GPIOC, 12},     /* EF_HAL_GPIO_SW_3 */
    {PORTC, GPIOC, 5},      /* EF_HAL_INT1_ACCEL */
    {PORTD, GPIOD, 1},      /* EF_HAL_INT2_ACCEL */

    {PORTD, GPIOD, 2},      /* EF_HAL_D9 */
    {PORTA, GPIOA, 13},     /* EF_HAL_D8 */
    {PORTC, GPIOC, 9},      /* EF_HAL_D7 */
    {PORTC, GPIOC, 8},      /* EF_HAL_D6 */
    {PORTA, GPIOA, 5},      /* EF_HAL_D5 */
    {PORTA, GPIOA, 4},      /* EF_HAL_D4 */
    {PORTA, GPIOA, 12},     /* EF_HAL_D3 */
    {PORTD, GPIOD, 3},      /* EF_HAL_D2 */

    {PORTB, GPIOB, 0},      /* EF_HAL_A0 */
    {PORTB, GPIOB, 1},      /* EF_HAL_A1 */
    {PORTB, GPIOB, 2},      /* EF_HAL_A2 */
    {PORTB, GPIOB, 3},      /* EF_HAL_A3 */
    {PORTC, GPIOC, 2},      /* EF_HAL_A4 */
    {PORTC, GPIOC, 1},      /* EF_HAL_A5 */
};

#define TOTAL_GPIO   (sizeof(gpioStruct) / sizeof(gpioStruct[0]))

static const efHal_gpio_id_t gpioOut[] =
{
    EF_HAL_GPIO_LED_GREEN,
    EF_HAL_GPIO_LED_RED,
};

static const uint32_t initialState = 0b0011;        /* initial pin state :
                                                     * 1: EF_HAL_GPIO_LED_GREEN
                                                     * 1: EF_HAL_GPIO_LED_RED
                                                     */

#define TOTAL_OUTPUTS   (sizeof(gpioOut) / sizeof(gpioOut[0]))

static const efHal_gpio_id_t gpioIn[] =
{
    EF_HAL_GPIO_SW_1,
    EF_HAL_GPIO_SW_3,
    EF_HAL_INT1_ACCEL,
    EF_HAL_INT2_ACCEL,
};

#define TOTAL_INPUTS   (sizeof(gpioIn) / sizeof(gpioIn[0]))

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void setPin(efHal_gpio_id_t id, bool state)
{
    if (state)
        GPIO_PortSet(gpioStruct[id].gpio, 1<<gpioStruct[id].pin);
    else
        GPIO_PortClear(gpioStruct[id].gpio, 1<<gpioStruct[id].pin);
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

        case EF_HAL_GPIO_INT_TYPE_LOW_LEVEL:
            portInt = kPORT_InterruptLogicZero;
            break;

        case EF_HAL_GPIO_INT_TYPE_HIGH_LEVEL:
            portInt = kPORT_InterruptLogicOne;
            break;

        default:
            efErrorHdl_error(EF_ERROR_HDL_INVALID_PARAMETER, "intType");
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
        efErrorHdl_error(EF_ERROR_HDL_INVALID_PARAMETER, "port");
    }
}

static void confPin(efHal_gpio_id_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull, bool state)
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

static void confBus(efHal_gpio_busid_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull)
{
    if (id == EF_HAL_BUS_TFT)
    {
        confPin(EF_HAL_D2, dir, pull, 0);
        confPin(EF_HAL_D3, dir, pull, 0);
        confPin(EF_HAL_D4, dir, pull, 0);
        confPin(EF_HAL_D5, dir, pull, 0);
        confPin(EF_HAL_D6, dir, pull, 0);
        confPin(EF_HAL_D7, dir, pull, 0);
        confPin(EF_HAL_D8, dir, pull, 0);
        confPin(EF_HAL_D9, dir, pull, 0);

        confPin(EF_HAL_BUS_WR, dir, pull, 1);
    }
    else
    {
        efErrorHdl_error(EF_ERROR_HDL_INVALID_PARAMETER, "busid");
    }
}

static void writeBus(efHal_gpio_busid_t id, void *pData, size_t length)
{
    uint8_t *dummy = pData;

    if (id == EF_HAL_BUS_TFT)
    {
        for (int i = 0 ; i < length ; i++)
        {
            uint8_t d = dummy[i];

            GPIO_PortClear(GPIOA, 0B0011000000110000);
            GPIO_PortClear(GPIOD, 0B0000000000001100);
            GPIO_PortClear(GPIOC, 0B0000001100000000);

            GPIO_PortSet(GPIOA, (uint32_t)(d & 0b00110000) | ((uint32_t)(d & 0b00000001) << 13) | ((uint32_t)(d & 0b00001000) << 9) );
            GPIO_PortSet(GPIOD, (uint32_t)(d & 0b00000110) << 1 );
            GPIO_PortSet(GPIOC, (uint32_t)(d & 0b11000000) << 2 );

            efHal_gpio_setPin(EF_HAL_BUS_WR, 0);
            efHal_gpio_setPin(EF_HAL_BUS_WR, 1);
        }
    }
    else
    {
        efErrorHdl_error(EF_ERROR_HDL_INVALID_PARAMETER, "busid");
    }
}


static void identifyAndNotifyGpioHandler(GPIO_Type *gpio, int pin)
{
    int i;

    for (i = 0 ; i < TOTAL_GPIO ; i++)
    {
        if (gpioStruct[i].gpio == gpio && gpioStruct[i].pin == pin)
        {
            efHal_internal_gpio_InterruptRoutine(i);
        }
    }
}

/*==================[external functions definition]==========================*/
extern void bsp_frdmkl46z_gpio_init(void)
{
    efHal_gpio_callBacks_t cb;
    int i;

    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);

    cb.setPin = setPin;
    cb.togPin = togPin;
    cb.getPin = getPin;
    cb.confInt = confInt;
    cb.confPin = confPin;
    cb.confBus = confBus;
    cb.writeBus = writeBus;

    efHal_internal_gpio_setCallBacks(cb);

    for (i = 0 ; i < TOTAL_OUTPUTS ; i++)
        confPin(gpioOut[i], EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, (initialState >> i) & 1);

    for (i = 0 ; i < TOTAL_INPUTS; i++)
        confPin(gpioIn[i], EF_HAL_GPIO_INPUT, EF_HAL_GPIO_PULL_DISABLE, 0);

    confPin(EF_HAL_GPIO_SW_1, EF_HAL_GPIO_INPUT, EF_HAL_GPIO_PULL_UP, 0);
    confPin(EF_HAL_GPIO_SW_3, EF_HAL_GPIO_INPUT, EF_HAL_GPIO_PULL_UP, 0);
}

void PORTC_PORTD_IRQHandler(void)
{
    uint32_t intFlags;
    static GPIO_Type * const gpioArr[] =
    {
        GPIOC,
        GPIOD,
    };
    static const int lengthGpioArr = sizeof(gpioArr) / sizeof(gpioArr[0]);
    int i,j;

    for (i = 0 ; i < lengthGpioArr ; i++)
    {
        for (j = 0 ; j < 32 ; j++)
        {
            intFlags = GPIO_PortGetInterruptFlags(gpioArr[i]);
            if (intFlags & (1 << j))
            {
                identifyAndNotifyGpioHandler(gpioArr[i], j);
                GPIO_PortClearInterruptFlags(gpioArr[i], 1 << j);
            }
        }
    }
}

void PORTA_IRQHandler(void)
{
    uint32_t intFlags;
    int j;

    for (j = 0 ; j < 32 ; j++)
    {
        intFlags = GPIO_PortGetInterruptFlags(GPIOA);
        if (intFlags | (1 << j))
        {
            identifyAndNotifyGpioHandler(GPIOA, j);
            GPIO_PortClearInterruptFlags(GPIOA, 1 << j);
        }
    }
}


/*==================[end of file]============================================*/
