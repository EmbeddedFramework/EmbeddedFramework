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

#define BUF_SIZE    (MY_DISP_VER_RES*12) // MY_DISP_HOR_RES * MY_DISP_VER_SER / 20

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[BUF_SIZE];
static lv_disp_drv_t disp_drv;        /*Descriptor of a display driver*/
static lv_indev_drv_t indev_drv;
static int timeDownTouch;

void lv_example_keyboard_2(void)
{
    /*Create an AZERTY keyboard map*/
    static const char * kb_map[] = {"A", "Z", "E", "R", "T", "Y", "U", "I", "O", "P", LV_SYMBOL_BACKSPACE, "\n",
                                    "Q", "S", "D", "F", "G", "J", "K", "L", "M",  LV_SYMBOL_NEW_LINE, "\n",
                                    "W", "X", "C", "V", "B", "N", ",", ".", ":", "!", "?", "\n",
                                    LV_SYMBOL_CLOSE, " ",  " ", " ", LV_SYMBOL_OK, NULL
                                   };

    /*Set the relative width of the buttons and other controls*/
    static const lv_btnmatrix_ctrl_t kb_ctrl[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6,
                                                  4, 4, 4, 4, 4, 4, 4, 4, 4, 6,
                                                  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                                                  2, LV_BTNMATRIX_CTRL_HIDDEN | 2, 6, LV_BTNMATRIX_CTRL_HIDDEN | 2, 2
                                                 };

    /*Create a keyboard and add the new map as USER_1 mode*/
    lv_obj_t * kb = lv_keyboard_create(lv_scr_act());

    lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
    lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_USER_1);

    /*Create a text area. The keyboard will write here*/
    lv_obj_t * ta;
    ta = lv_textarea_create(lv_scr_act());
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_size(ta, lv_pct(90), 80);
    lv_obj_add_state(ta, LV_STATE_FOCUSED);

    lv_keyboard_set_textarea(kb, ta);
}

static void blinky_task(void *pvParameters)
{
    ili9486_init(DISPLAY_ORIENTATION_HORIZONTAL, ILI9486_DC, ILI9486_RST, ILI9486_CS, ILI9486_BUS);

    touchScreen_init(TFT_XM, TFT_XP, TFT_YM, TFT_YP);
    touchScreen_conf(350, 3660, 550, 3558, MY_DISP_HOR_RES, MY_DISP_VER_RES);
    touchScreen_swapXY(1);

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

    lv_example_keyboard_2();

    for (;;)
    {
        if (timeDownTouch == 0)
        {
            timeDownTouch = 20;
            touchScreen_performRead();
        }

        efHal_gpio_confPin(TFT_YP, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
        efHal_gpio_confPin(TFT_YM, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
        efHal_gpio_confPin(TFT_XP, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
        efHal_gpio_confPin(TFT_XM, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);

        lv_timer_handler();
    }
}

/*==================[external functions definition]==========================*/
int main(void)
{
    appBoard_init();

    xTaskCreate(blinky_task, "blinky_task", 500, NULL, 0, NULL);

    vTaskStartScheduler();
    for (;;);
}

extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
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
