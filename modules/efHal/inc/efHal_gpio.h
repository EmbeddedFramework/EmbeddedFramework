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
#ifndef EF_HAL_GPIO_H_
#define EF_HAL_GPIO_H_

/*==================[inclusions]=============================================*/
#include "stdint.h"
#include "stdbool.h"
#include "FreeRTOS.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

typedef int32_t efHal_gpio_id_t;
typedef int32_t efHal_gpio_busid_t;

typedef enum
{
    EF_HAL_GPIO_INT_TYPE_DISABLE = 0,
    EF_HAL_GPIO_INT_TYPE_RISING_EDGE,
    EF_HAL_GPIO_INT_TYPE_FALLING_EDGE,
    EF_HAL_GPIO_INT_TYPE_BOTH_EDGE,
    EF_HAL_GPIO_INT_TYPE_LOW_LEVEL,
    EF_HAL_GPIO_INT_TYPE_HIGH_LEVEL,
}efHal_gpio_intType_t;

typedef enum
{
    EF_HAL_GPIO_PULL_DISABLE = 0,
    EF_HAL_GPIO_PULL_UP,
    EF_HAL_GPIO_PULL_DOWN,
}efHal_gpio_pull_t;

typedef enum
{
    EF_HAL_GPIO_INPUT = 0,
    EF_HAL_GPIO_OUTPUT,
    EF_HAL_GPIO_OUTPUT_OD,      /* Output Open Drain */
}efHal_gpio_dir_t;

typedef void (*efHal_gpio_callBackInt_t)(efHal_gpio_id_t id);

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void efHal_gpio_init(void);
extern void efHal_gpio_setPin(efHal_gpio_id_t id, bool state);
extern void efHal_gpio_togglePin(efHal_gpio_id_t id);
extern bool efHal_gpio_getPin(efHal_gpio_id_t id);
extern void efHal_gpio_confInt(efHal_gpio_id_t id, efHal_gpio_intType_t intType);
extern void efHal_gpio_confPin(efHal_gpio_id_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull, bool state);
extern bool efHal_gpio_waitForInt(efHal_gpio_id_t id, TickType_t xBlockTime);
extern void efHal_gpio_setCallBackInt(efHal_gpio_id_t id, efHal_gpio_callBackInt_t cb);
extern void efHal_gpio_confBus(efHal_gpio_busid_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull);
extern void efHal_gpio_writeBus(efHal_gpio_busid_t id, void *pData, size_t length);
extern void efHal_gpio_readBus(efHal_gpio_busid_t id, void *pData, size_t length);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* EF_HAL_GPIO_H_ */
