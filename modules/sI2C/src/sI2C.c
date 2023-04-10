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
#include "sI2C.h"
#include "efHal.h"

#if __has_include("sI2C_config.h")
    #include "sI2C_config.h"
#endif

/*==================[macros and typedef]=====================================*/

#ifndef sI2C_TOTAL
    #define sI2C_TOTAL     (1)
#endif

#ifndef sI2C_PERIOD
    #define sI2C_PERIOD              10      /* 10 microseconds -> 100000 khz */
#endif

#define SEMI_PERIOD         (sI2C_PERIOD / 2)

typedef struct
{
    efHal_gpio_id_t scl;
    efHal_gpio_id_t sda;
    sI2C_delay_t delay;
}sI2C_data_t;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static sI2C_data_t sI2C_data[sI2C_TOTAL];

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static inline void gpioLow(efHal_gpio_id_t gpio)
{
    efHal_gpio_confPin(gpio, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 0);
}

static inline void gpioHigh(efHal_gpio_id_t gpio)
{
    efHal_gpio_confPin(gpio, EF_HAL_GPIO_INPUT, EF_HAL_GPIO_PULL_DISABLE, 0);
}


static void sendStart(sI2C_data_t *sI2C_data)
{
    gpioHigh(sI2C_data->scl);
    gpioHigh(sI2C_data->sda);

    sI2C_data->delay(SEMI_PERIOD);
    gpioLow(sI2C_data->sda);
    sI2C_data->delay(SEMI_PERIOD);
    gpioLow(sI2C_data->scl);
    sI2C_data->delay(SEMI_PERIOD);
}

static void sendStop(sI2C_data_t *sI2C_data)
{
    gpioLow(sI2C_data->scl);
    gpioLow(sI2C_data->sda);
    sI2C_data->delay(SEMI_PERIOD);
    gpioHigh(sI2C_data->scl);
    sI2C_data->delay(SEMI_PERIOD);
    gpioHigh(sI2C_data->sda);
}

static bool waitACK(sI2C_data_t *sI2C_data)
{
    bool ret = true;
    uint8_t timeout = 200;

    gpioHigh(sI2C_data->sda);
    sI2C_data->delay(SEMI_PERIOD);
    gpioHigh(sI2C_data->scl);

    while (timeout--)
    {
        sI2C_data->delay(SEMI_PERIOD);
        if (efHal_gpio_getPin(sI2C_data->sda) == 0)
        {
            ret = 0;
            break;
        }
    }

    gpioLow(sI2C_data->scl);

    return ret;
}

static void sendAck(sI2C_data_t *sI2C_data)
{
    gpioLow(sI2C_data->scl);
    gpioLow(sI2C_data->sda);
    sI2C_data->delay(SEMI_PERIOD);
    gpioHigh(sI2C_data->scl);
    sI2C_data->delay(SEMI_PERIOD);
    gpioLow(sI2C_data->scl);
}

static void sendNAck(sI2C_data_t *sI2C_data)
{
    gpioLow(sI2C_data->scl);
    gpioHigh(sI2C_data->sda);
    sI2C_data->delay(SEMI_PERIOD);
    gpioHigh(sI2C_data->scl);
    sI2C_data->delay(SEMI_PERIOD);
    gpioLow(sI2C_data->scl);
}

/*==================[external functions definition]==========================*/

extern void sI2C_init(void)
{
    int i;

    for (i = 0 ; i < sI2C_TOTAL ; i++)
    {
        sI2C_data[i].scl = EF_HAL_INVALID_ID;
        sI2C_data[i].sda = EF_HAL_INVALID_ID;
    }
}

extern void* sI2C_open(efHal_gpio_id_t scl, efHal_gpio_id_t sda, sI2C_delay_t delay)
{
    void* ret = NULL;

    int i;

    for (i = 0 ; i < sI2C_TOTAL ; i++)
    {
        if (sI2C_data[i].scl == EF_HAL_INVALID_ID)
        {
            ret = &sI2C_data[i];

            sI2C_data[i].scl = scl;
            sI2C_data[i].sda = sda;
            sI2C_data[i].delay = delay;

            efHal_gpio_confPin(scl, EF_HAL_GPIO_INPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
            efHal_gpio_confPin(sda, EF_HAL_GPIO_INPUT, EF_HAL_GPIO_PULL_DISABLE, 1);

            break;
        }
    }

    return ret;
}

/*==================[end of file]============================================*/
