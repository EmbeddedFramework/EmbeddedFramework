/*
###############################################################################
#
# Copyright 2023, Gustavo Muro
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
#include "bsp_nucleoF767ZI_gpio.h"
#include "efHal_internal.h"

#include "main.h"

/*==================[macros and typedef]=====================================*/
typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
}gpioStruct_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static const gpioStruct_t gpioStruct[] =
{
    {LD1_GPIO_Port,  LD1_Pin},              /* EF_HAL_GPIO_LED_LD1 */
    {LD2_GPIO_Port,  LD2_Pin},              /* EF_HAL_GPIO_LED_LD2 */
    {LD3_GPIO_Port,  LD3_Pin},              /* EF_HAL_GPIO_LED_LD3 */
    {USER_Btn_GPIO_Port,  USER_Btn_Pin},    /* EF_HAL_GPIO_BUTTON_B1 */

    {GPIOB, GPIO_PIN_8},                    /* EF_HAL_D15 */
    {GPIOB, GPIO_PIN_9},                    /* EF_HAL_D14 */
    {GPIOA, GPIO_PIN_5},                    /* EF_HAL_D13 */
    {GPIOA, GPIO_PIN_6},                    /* EF_HAL_D12 */
    {GPIOA, GPIO_PIN_7},                    /* EF_HAL_D11 */
    {GPIOD, GPIO_PIN_14},                   /* EF_HAL_D10 */
    {GPIOD, GPIO_PIN_15},                   /* EF_HAL_D9 */
    {GPIOF, GPIO_PIN_12},                   /* EF_HAL_D8 */
    {GPIOF, GPIO_PIN_13},                   /* EF_HAL_D7 */
    {GPIOE, GPIO_PIN_9},                    /* EF_HAL_D6 */
    {GPIOE, GPIO_PIN_11},                   /* EF_HAL_D5 */
    {GPIOF, GPIO_PIN_14},                   /* EF_HAL_D4 */
    {GPIOE, GPIO_PIN_13},                   /* EF_HAL_D3 */
    {GPIOF, GPIO_PIN_15},                   /* EF_HAL_D2 */
    {GPIOG, GPIO_PIN_14},                   /* EF_HAL_D1 */
    {GPIOG, GPIO_PIN_9},                    /* EF_HAL_D0 */

    {GPIOA, GPIO_PIN_3},                    /* EF_HAL_A0 */
    {GPIOC, GPIO_PIN_0},                    /* EF_HAL_A1 */
    {GPIOC, GPIO_PIN_3},                    /* EF_HAL_A2 */
    {GPIOF, GPIO_PIN_3},                    /* EF_HAL_A3 */
    {GPIOF, GPIO_PIN_5},                    /* EF_HAL_A4 */
    {GPIOF, GPIO_PIN_10},                   /* EF_HAL_A5 */

};

#define TOTAL_GPIO   (sizeof(gpioStruct) / sizeof(gpioStruct[0]))

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void setPin(efHal_gpio_id_t id, bool state)
{
    HAL_GPIO_WritePin(gpioStruct[id].GPIOx, gpioStruct[id].GPIO_Pin, state);
}

static void togPin(efHal_gpio_id_t id)
{
    HAL_GPIO_TogglePin(gpioStruct[id].GPIOx, gpioStruct[id].GPIO_Pin);
}

static bool getPin(efHal_gpio_id_t id)
{
    return HAL_GPIO_ReadPin(gpioStruct[id].GPIOx, gpioStruct[id].GPIO_Pin);
}

static void confInt(efHal_gpio_id_t id, efHal_gpio_intType_t intType)
{

}

static void confPin(efHal_gpio_id_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull, bool state)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    setPin(id, state);

    switch (pull)
    {
        case EF_HAL_GPIO_PULL_DISABLE:
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;

        case EF_HAL_GPIO_PULL_UP:
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            break;

        case EF_HAL_GPIO_PULL_DOWN:
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            break;
    }

    switch (dir)
    {
        case EF_HAL_GPIO_INPUT:
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            break;

        case EF_HAL_GPIO_OUTPUT:
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            break;

        case EF_HAL_GPIO_OUTPUT_OD:
            GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
            break;
    }

    GPIO_InitStruct.Pin = gpioStruct[id].GPIO_Pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gpioStruct[id].GPIOx, &GPIO_InitStruct);
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
        confPin(EF_HAL_BUS_RD, dir, pull, 1);
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

            // clear bits
            // bit 0 D8 F12
            // bit 2 D2 F15
            // bit 4 D4 F14
            // bit 7 D7 F13
            GPIOF->BSRR = (GPIO_PIN_12|GPIO_PIN_15|GPIO_PIN_14|GPIO_PIN_13) << 16;

            // bit 1 D9 D15
            GPIOD->BSRR = (GPIO_PIN_15) << 16;

            // bit 3 D3 E13
            // bit 5 D5 E11
            // bit 6 D6 E9
            GPIOE->BSRR = (GPIO_PIN_13|GPIO_PIN_11|GPIO_PIN_9) << 16;

            // set bits
            if (d & 0b00000001)
                GPIOF->BSRR = GPIO_PIN_12;
            if (d & 0b00000100)
                GPIOF->BSRR = GPIO_PIN_15;
            if (d & 0b00010000)
                GPIOF->BSRR = GPIO_PIN_14;
            if (d & 0b10000000)
                GPIOF->BSRR = GPIO_PIN_13;

            if (d & 0b00000010)
                GPIOD->BSRR = GPIO_PIN_15;

            if (d & 0b00001000)
                GPIOE->BSRR = GPIO_PIN_13;
            if (d & 0b00100000)
                GPIOE->BSRR = GPIO_PIN_11;
            if (d & 0b01000000)
                GPIOE->BSRR = GPIO_PIN_9;

            efHal_gpio_setPin(EF_HAL_BUS_WR, 0);
            efHal_gpio_setPin(EF_HAL_BUS_WR, 1);
        }
    }
    else
    {
        efErrorHdl_error(EF_ERROR_HDL_INVALID_PARAMETER, "busid");
    }
}


/*==================[external functions definition]==========================*/
extern void bsp_nucleoF767ZI_gpio_init(void)
{
    efHal_gpio_callBacks_t cb;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    cb.setPin = setPin;
    cb.togPin = togPin;
    cb.getPin = getPin;
    cb.confInt = confInt;
    cb.confPin = confPin;
    cb.confBus = confBus;
    cb.writeBus = writeBus;

    efHal_internal_gpio_setCallBacks(cb);
}

extern void bsp_nucleoF767ZI_internal_gpio_confAsAnalog(efHal_gpio_id_t id)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pin = gpioStruct[id].GPIO_Pin;

    HAL_GPIO_Init(gpioStruct[id].GPIOx, &GPIO_InitStruct);
}

extern void bsp_nucleoF767ZI_internal_gpio_confAsPWM(efHal_gpio_id_t id)
{

}


/*==================[end of file]============================================*/
