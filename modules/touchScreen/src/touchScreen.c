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

/*==================[macros and typedef]=====================================*/

/*=================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static efHal_gpio_id_t gpioXM;
static efHal_gpio_id_t gpioXP;
static efHal_gpio_id_t gpioYM;
static efHal_gpio_id_t gpioYP;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*Return true is the touchpad is pressed*/
static bool touchpad_is_pressed(void)
{
    /*Your code comes here*/

    return false;
}

/*Get the x and y coordinates if the touchpad is pressed*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
    /*Your code comes here*/

    (*x) = 0;
    (*y) = 0;
}

static void setup_gpio(efHal_gpio_id_t plus, efHal_gpio_id_t minus, efHal_gpio_id_t measure, efHal_gpio_id_t ignore)
{

}
/*==================[external functions definition]==========================*/

extern void touchScreen_init(efHal_gpio_id_t xm, efHal_gpio_id_t xp,
        efHal_gpio_id_t ym, efHal_gpio_id_t yp)
{
    gpioXM = xm;
    gpioXP = xp;
    gpioYM = ym;
    gpioYP = yp;
}

extern void touchScreen_performRead(void)
{
    int x, y;

    efHal_analog_confAsAnalog(gpioYP);
    efHal_gpio_confPin(gpioYM, EF_HAL_GPIO_INPUT, EF_HAL_GPIO_PULL_DISABLE, 1);

    efHal_gpio_confPin(gpioXP, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
    efHal_gpio_confPin(gpioXM, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 0);

    efHal_analog_startConv(gpioYP);
    efHal_analog_waitConv(gpioYP, portMAX_DELAY);
    x = efHal_analog_read(gpioYP);

    x = (1023 - x);

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
