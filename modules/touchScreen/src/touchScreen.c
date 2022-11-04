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
#include "touchScreen.h"
#include "efHal_analog.h"
#include "stdlib.h"

/*==================[macros and typedef]=====================================*/

#define SAMPLES_TOTAL        2
#define SAMPLES_MAX_DIFF    16

/*=================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static efHal_gpio_id_t gpioXM;
static efHal_gpio_id_t gpioXP;
static efHal_gpio_id_t gpioYM;
static efHal_gpio_id_t gpioYP;
static int32_t thresholdValid;
static int32_t readX;
static int32_t readY;

struct
{
    unsigned pressed:1;
    unsigned swapXY:1;
    unsigned enablePullUp:1;
}flags;

static int32_t minX = 0, maxX = 1024, minY = 0, maxY = 1024;
static int32_t resX = 480, resY = 320;


/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void)
{
    return flags.pressed;
}

static int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    *x = map(readX, minX, maxX, 0, resX);
    *y = map(readY, minY, maxY, 0, resY);
}

static int32_t getValue(efHal_gpio_id_t plus, efHal_gpio_id_t minus, efHal_gpio_id_t measure, efHal_gpio_id_t ignore)
{
    int32_t samples[SAMPLES_TOTAL];
    int32_t ret;
    int i;

    efHal_analog_confAsAnalog(measure);
    efHal_gpio_confPin(ignore, EF_HAL_GPIO_INPUT,
            flags.enablePullUp?EF_HAL_GPIO_PULL_UP:EF_HAL_GPIO_PULL_DISABLE, 1);

    efHal_gpio_confPin(plus, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
    efHal_gpio_confPin(minus, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 0);

    for (i = 0 ; i < SAMPLES_TOTAL ; i++)
    {
        efHal_analog_startConv(measure);
        efHal_analog_waitConv(measure, portMAX_DELAY);
        samples[i] = efHal_analog_read(measure);
    }

    ret = samples[0];
    for (i = 1 ; i < SAMPLES_TOTAL && ret >= 0; i++)
    {
        if (abs(samples[i-1] - samples[i]) > SAMPLES_MAX_DIFF)
            ret = -1;
        else
            ret += samples[i];
    }

    if (ret >= 0)
    {
        ret = ret / SAMPLES_TOTAL;

        if (ret >= thresholdValid)
            ret = -1;
    }

    return ret;
}

/*==================[external functions definition]==========================*/

extern void touchScreen_init(efHal_gpio_id_t xm, efHal_gpio_id_t xp,
        efHal_gpio_id_t ym, efHal_gpio_id_t yp)
{
    gpioXM = xm;
    gpioXP = xp;
    gpioYM = ym;
    gpioYP = yp;

    thresholdValid = (efHal_analog_getFullValue(0) * 97) / 100;
}

extern void touchScreen_conf(int32_t mix, int32_t max, int32_t miy, int32_t may,
        int32_t rx, int32_t ry)
{
    minX = mix;
    maxX = max;
    minY = miy;
    maxY = may;
    resX = rx;
    resY = ry;
}

extern void touchScreen_swapXY(bool swXY)
{
    flags.swapXY = swXY;
}

extern void touchScreen_enablePullUP(bool enPU)
{
    flags.enablePullUp = enPU;
}

extern void touchScreen_performRead(void)
{
    int32_t x, y;

    x = getValue(gpioXP, gpioXM, gpioYP, gpioYM);
    y = getValue(gpioYP, gpioYM, gpioXP, gpioXM);

    if (flags.swapXY)
    {
        readY = x;
        readX = y;
    }
    else
    {
        readX = x;
        readY = y;
    }

    if (readX >= 0 && readY >= 0)
        flags.pressed = true;
    else
        flags.pressed = false;
}


extern void touchScreen_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /*Save the pressed coordinates and the state*/
    if(touchpad_is_pressed()) {
        touchpad_get_xy(&last_x, &last_y);
        data->state = LV_INDEV_STATE_PR;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;
}

/*==================[end of file]============================================*/
