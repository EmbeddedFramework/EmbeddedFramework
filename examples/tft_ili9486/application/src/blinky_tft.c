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
#include "appBoard.h"
#include "efHal_gpio.h"
#include "FreeRTOS.h"
#include "task.h"

#include "ili9486.h"
#include "touchScreen.h"
#include "lvgl.h"

/*==================[macros and typedef]=====================================*/
#define MY_DISP_VER_RES 320
#define MY_DISP_HOR_RES 480

#define BUF_SIZE    (MY_DISP_VER_RES*10) // MY_DISP_HOR_RES * MY_DISP_VER_SER / 20

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[BUF_SIZE];
static lv_disp_drv_t disp_drv;        /*Descriptor of a display driver*/
static lv_indev_drv_t indev_drv;
static int timeDownTouch;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

void lv_example_list_2(void);

static void blinky_task(void *pvParameters)
{
    ili9486_init(DISPLAY_ORIENTATION_HORIZONTAL, ILI9486_DC, ILI9486_RST, ILI9486_CS, ILI9486_BUS);

    touchScreen_init(TFT_XM, TFT_XP, TFT_YM, TFT_YP);
    touchScreen_conf(350, 3660, 550, 3558, MY_DISP_HOR_RES, MY_DISP_VER_RES);
    touchScreen_swapXY(1);
    touchScreen_enablePullUP(1);

    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, BUF_SIZE);  /*Initialize the display buffer.*/

    lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
    disp_drv.flush_cb = ili9486_flush;    /*Set your driver function*/
    disp_drv.draw_buf = &draw_buf;        /*Assign the buffer to the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;   /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = MY_DISP_VER_RES;   /*Set the vertical resolution of the display*/
    lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/

    lv_indev_drv_init(&indev_drv);        /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchScreen_read;
    lv_indev_drv_register(&indev_drv);    /*Register the driver in LVGL */

    lv_example_list_2();

    for (;;)
    {
        if (timeDownTouch == 0)
        {
            timeDownTouch = 20;
            touchScreen_performRead();
            efHal_gpio_confPin(TFT_YP, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
            efHal_gpio_confPin(TFT_YM, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
            efHal_gpio_confPin(TFT_XP, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
            efHal_gpio_confPin(TFT_XM, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
        }

        lv_timer_handler();
    }
}

/*==================[external functions definition]==========================*/
void app_init(void)
{
    xTaskCreate(blinky_task, "blinky_task", 600, NULL, 0, NULL);
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (1);
}

extern void vApplicationMallocFailedHook( void )
{
    while (1);
}

void vApplicationTickHook(void)
{
    lv_tick_inc(1);

    if (timeDownTouch)
        timeDownTouch--;
}

/*==================[end of file]============================================*/
