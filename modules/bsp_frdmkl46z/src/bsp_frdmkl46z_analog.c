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
#include "fsl_adc16.h"

/*==================[macros and typedef]=====================================*/

typedef struct
{
    efHal_gpio_id_t gpioId;
    uint32_t channel;
}analogAsign_t;


/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static const analogAsign_t map[] =
{
        { EF_HAL_A0, 8 },
        { EF_HAL_A1, 9 },
        { EF_HAL_A2, 12 },
        { EF_HAL_A3, 13 },
};

#define MAP_LENGTH  (sizeof(map) / sizeof(map[0]))

static adc16_channel_config_t adc16_channel;
static int indexMap;
static uint32_t adcVal;
static SemaphoreHandle_t mutex;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

extern void bsp_frdmkl46z_internal_gpio_confAsAnalog(efHal_gpio_id_t id);

static int getIndexMap(efHal_gpio_id_t id)
{
    int ret = -1;

    for (int i = 0 ; i < MAP_LENGTH && ret == -1 ; i++)
    {
        if (map[i].gpioId == id)
            ret = i;
    }

    return ret;
}

static bool startConv(efHal_gpio_id_t id)
{
    bool ret = false;

    xSemaphoreTake(mutex, portMAX_DELAY);

    indexMap = getIndexMap(id);

    if (indexMap >= 0)
    {
        adc16_channel.channelNumber = map[indexMap].channel;
        ADC16_SetChannelConfig(ADC0, 0, &adc16_channel);
        ret = true;
    }
    else
    {
        xSemaphoreGive(mutex);
    }

    return ret;
}

static int32_t aRead(efHal_gpio_id_t id)
{
    int32_t ret = adcVal;
    xSemaphoreGive(mutex);
    return ret;
}

static int32_t getFullValue(efHal_gpio_id_t id)
{
    return 4095;
}

/*==================[external functions definition]==========================*/
extern void bsp_frdmkl46z_analog_init(void)
{
    efHal_analog_callBacks_t cb;
    adc16_config_t adc16_config;

    mutex = xSemaphoreCreateMutex();

    ADC16_GetDefaultConfig(&adc16_config);
    ADC16_Init(ADC0, &adc16_config);
    ADC16_EnableHardwareTrigger(ADC0, false); /* Make sure the software trigger is used. */

    adc16_channel.channelNumber = 0;
    adc16_channel.enableInterruptOnConversionCompleted = true; /* Enable the interrupt. */
    adc16_channel.enableDifferentialConversion = false;

    NVIC_EnableIRQ(ADC0_IRQn);

    cb.confAsAnalog = bsp_frdmkl46z_internal_gpio_confAsAnalog;
    cb.startConv = startConv;
    cb.aRead = aRead;
    cb.getFullValue = getFullValue;

    efHal_internal_analog_setCallBacks(cb);
}

void ADC0_IRQHandler(void)
{
    /* Read conversion result to clear the conversion completed flag. */
    adcVal = ADC16_GetChannelConversionValue(ADC0, 0);

    efHal_internal_analog_endConvInterruptRoutine(map[indexMap].gpioId);
}

/*==================[end of file]============================================*/
