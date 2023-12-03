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
#ifndef BSP_NUCLEO_F767ZI_GPIO_H
#define BSP_NUCLEO_F767ZI_GPIO_H

/*==================[inclusions]=============================================*/
#include "efHal_gpio.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

enum efHal_gpio_id_t
{
    EF_HAL_GPIO_LED_LD1 = 0,
	EF_HAL_GPIO_LED_LD2,
	EF_HAL_GPIO_LED_LD3,
    EF_HAL_GPIO_BUTTON_B1,

    EF_HAL_D15,
    EF_HAL_D14,
    EF_HAL_D13,
    EF_HAL_D12,
    EF_HAL_D11,
    EF_HAL_D10,
    EF_HAL_D9,
    EF_HAL_D8,
    EF_HAL_D7,
    EF_HAL_D6,
    EF_HAL_D5,
    EF_HAL_D4,
    EF_HAL_D3,
    EF_HAL_D2,
    EF_HAL_D1,
    EF_HAL_D0,

    EF_HAL_A0,
    EF_HAL_BUS_RD = EF_HAL_A0,
    EF_HAL_A1,
    EF_HAL_BUS_WR = EF_HAL_A1,
    EF_HAL_A2,
    EF_HAL_A3,
    EF_HAL_A4,
    EF_HAL_A5,
};

enum efHal_gpio_busid_t
{
    EF_HAL_BUS_TFT = 0,
};

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void bsp_nucleoF767ZI_gpio_init(void);
extern void bsp_nucleoF767ZI_internal_gpio_confAsPWM(efHal_gpio_id_t id);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* BSP_NUCLEO_F767ZI_GPIO_H */
