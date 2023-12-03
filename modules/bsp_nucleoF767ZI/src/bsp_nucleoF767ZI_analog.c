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
#include "bsp_nucleoF767ZI_analog.h"
#include "bsp_nucleoF767ZI_gpio.h"
#include "efHal_internal.h"

#include "efHal.h"
#include "efHal_gpio.h"
#include "main.h"

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
        { EF_HAL_A0, ADC_CHANNEL_3 },
        { EF_HAL_A1, ADC_CHANNEL_10 },
        { EF_HAL_A2, ADC_CHANNEL_13 },
};

#define MAP_LENGTH  (sizeof(map) / sizeof(map[0]))

static int indexMap;
static uint32_t adcVal[MAP_LENGTH];
static SemaphoreHandle_t semphAdc;

/*==================[external data definition]===============================*/
extern ADC_HandleTypeDef hadc1;

/*==================[internal functions definition]==========================*/

extern void bsp_nucleoF767ZI_internal_gpio_confAsAnalog(efHal_gpio_id_t id);

static void setChannel(uint32_t channel, ADC_HandleTypeDef *adc)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK)
    {
        efErrorHdl_error(EF_ERROR_HDL_APPLICATION, "analog.setChannel");
    }
}

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

    xSemaphoreTake(semphAdc, portMAX_DELAY);

    indexMap = getIndexMap(id);

    if (indexMap >= 0)
    {
        setChannel(map[indexMap].channel, &hadc1);
        HAL_ADC_Start_IT(&hadc1);
        ret = true;
    }
    else
    {
        xSemaphoreGive(semphAdc);
    }

    return ret;
}

static int32_t aRead(efHal_gpio_id_t id)
{
    int32_t ret = getIndexMap(id);

    if (ret >= 0)
        ret = adcVal[ret];

    return ret;
}

static int32_t getFullValue(efHal_gpio_id_t id)
{
    return 4095;
}

/*==================[external functions definition]==========================*/
extern void bsp_nucleoF767ZI_analog_init(void)
{
    efHal_analog_callBacks_t cb;

    semphAdc = xSemaphoreCreateBinary();
    xSemaphoreGive(semphAdc);

    cb.confAsAnalog = bsp_nucleoF767ZI_internal_gpio_confAsAnalog;
    cb.startConv = startConv;
    cb.aRead = aRead;
    cb.getFullValue = getFullValue;

    efHal_internal_analog_setCallBacks(cb);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        /* Read conversion result to clear the conversion completed flag. */
        adcVal[indexMap] =HAL_ADC_GetValue(hadc);

        xSemaphoreGiveFromISR(semphAdc, &xHigherPriorityTaskWoken);

        efHal_internal_analog_endConvInterruptRoutine(map[indexMap].gpioId);

        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

    }
}

/*==================[end of file]============================================*/
