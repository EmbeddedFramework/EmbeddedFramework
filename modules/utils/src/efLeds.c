/*
###############################################################################
#
# Copyright 2022, Gustavo Muro
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
#include "efLeds.h"
#include "timers.h"

/*==================[macros and typedef]=====================================*/

#define TIME_BASE   10 // 10 ms

#define toTimeBase(time)    (time / TIME_BASE)

/*=================[internal functions declaration]=========================*/

typedef struct
{
    bool repeat;
    uint16_t time[];
}secuence_t;


typedef struct
{
    secuence_t const * pSec;
    int8_t secIndex;
    uint16_t timOut;
}ledState_t;

/*==================[internal data definition]===============================*/

static efLeds_conf_t const *conf;
static int totalLeds;
static ledState_t *pLedState;

static const secuence_t blinkySec =
{
    .repeat = true,
    .time =
    {
        toTimeBase(500),
        toTimeBase(500),
        0,
    },
};

static const secuence_t heartBeatSec =
{
    .repeat = true,
    .time =
    {
        toTimeBase(200),
        toTimeBase(200),
        toTimeBase(200),
        toTimeBase(800),
        0,
    },
};

static const secuence_t pulseSec =
{
    .repeat = false,
    .time =
    {
        toTimeBase(40),
        toTimeBase(10),
        0,
    },
};

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void vCB_leds( TimerHandle_t xTimer )
{
    int i;

    for (i = 0 ; i < totalLeds ; i++)
    {
        if (pLedState[i].pSec != NULL)
        {
            if (pLedState[i].timOut)
                pLedState[i].timOut--;
            else
            {
                pLedState[i].secIndex++;

                if ( (pLedState[i].pSec->time[pLedState[i].secIndex] == 0) &&
                     (pLedState[i].pSec->repeat) )
                {
                    pLedState[i].secIndex = 0;
                }

                if (pLedState[i].pSec->time[pLedState[i].secIndex] != 0)
                {
                    efHal_gpio_togglePin(conf[i].gpioId);
                    pLedState[i].timOut = pLedState[i].pSec->time[pLedState[i].secIndex];
                }
                else
                {
                    pLedState[i].secIndex = 0;
                    pLedState[i].pSec = NULL;
                }
            }
        }
    }
}

/*==================[external functions definition]==========================*/
extern void efLeds_init(efLeds_conf_t const *cf, int tl)
{
    int i;
    TimerHandle_t xTimer;

    conf = cf;
    totalLeds = tl;

    pLedState = pvPortMalloc(totalLeds * sizeof(ledState_t));

    for (i = 0 ; i < totalLeds ; i++)
    {
        pLedState[i].pSec = NULL;

        efHal_gpio_confPin(conf[i].gpioId, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, !conf[i].onState);

        efLeds_msg(i, EF_LEDS_MSG_OFF);
    }

    xTimer = xTimerCreate("efLeds",
            TIME_BASE / portTICK_PERIOD_MS,
            pdTRUE,
            NULL,
            vCB_leds);

    xTimerStart(xTimer, portMAX_DELAY);
}

extern void efLeds_msg(efLeds_id_t id, efLeds_msg_t msg)
{
    switch (msg)
    {
        case EF_LEDS_MSG_OFF:
            efHal_gpio_setPin(conf[id].gpioId, !conf[id].onState);
            break;

        case EF_LEDS_MSG_ON:
            efHal_gpio_setPin(conf[id].gpioId, conf[id].onState);
            break;

        case EF_LEDS_MSG_BLINKY:
            pLedState[id].timOut = blinkySec.time[0];
            pLedState[id].secIndex = 0;
            efHal_gpio_setPin(conf[id].gpioId, conf[id].onState);
            pLedState[id].pSec = &blinkySec;
            break;

        case EF_LEDS_MSG_HEARTBEAT:
            pLedState[id].timOut = heartBeatSec.time[0];
            pLedState[id].secIndex = 0;
            efHal_gpio_setPin(conf[id].gpioId, conf[id].onState);
            pLedState[id].pSec = &heartBeatSec;
            break;

        case EF_LEDS_MSG_PULSE:
            if (pLedState[id].pSec !=  &pulseSec)
            {
                pLedState[id].timOut = pulseSec.time[0];
                pLedState[id].secIndex = 0;
                efHal_gpio_setPin(conf[id].gpioId, conf[id].onState);
                pLedState[id].pSec = &pulseSec;
            }
            break;
    }
}

/*==================[end of file]============================================*/
