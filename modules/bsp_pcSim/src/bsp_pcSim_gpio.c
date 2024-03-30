/*
###############################################################################
#
# Copyright 2024, Gustavo Muro
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
#include "efHal_internal.h"
#include "bsp_pcSim.h"
#include "stdio.h"
#include "string.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static int8_t gpioState[EF_HAL_GPIO_TOTAL];

static char *gpioName[] =
{
    "GPIO_LED_RED",
    "GPIO_LED_GREEN",
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void setPin(efHal_gpio_id_t id, bool state)
{
    if (id >= EF_HAL_GPIO_TOTAL)
    {
        PRINT_DEBUG("ERROR: setPin id = %d\n", id);
        return;
    }

    if (gpioState[id] != state)
    {
        gpioState[id] = state;
        PRINT_DEBUG("%s = %d\n", gpioName[id], state);
    }
}

static bool getPin(efHal_gpio_id_t id)
{
    return gpioState[id];
}

static void confInt(efHal_gpio_id_t id, efHal_gpio_intType_t intType)
{
}

static void confPin(efHal_gpio_id_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull, bool state)
{
}

/*==================[external functions definition]==========================*/
extern void bsp_pcSim_gpio_init(void)
{
    efHal_gpio_callBacks_t cb;
    int i;

    for (i = 0 ; i < EF_HAL_GPIO_TOTAL ; i++)
        gpioState[i] = -1;

    cb.setPin = setPin;
    cb.togPin = NULL;
    cb.getPin = getPin;
    cb.confInt = confInt;
    cb.confPin = confPin;
    cb.confBus = NULL;
    cb.writeBus = NULL;

    efHal_internal_gpio_setCallBacks(cb);
}

/*==================[end of file]============================================*/
