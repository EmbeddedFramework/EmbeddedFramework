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
/* based on https://github.com/lvgl/lvgl_esp32_drivers */

/*********************
 *      INCLUDES
 *********************/
#include "ili9486.h"
#include "FreeRTOS.h"
#include "task.h"


#include "efHal_gpio.h"
#include "bsp_frdmkl46z_gpio.h"

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "pin_mux.h"


/*********************
 *      DEFINES
 *********************/
 #define TAG "ILI9486"

/**********************
 *      TYPEDEFS
 **********************/

/*The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct. */
typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void ili9486_set_orientation(uint8_t orientation);

static void ili9486_send_cmd(uint8_t cmd);
static void ili9486_send_data(void * data, uint32_t length);


/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ili9486_init(void)
{
	lcd_init_cmd_t ili_init_cmds[]={
		{0x11, {0}, 0x80},
		{0x3A, {0x55}, 1},
		{0x2C, {0x44}, 1},
		{0xC5, {0x00, 0x00, 0x00, 0x00}, 4},
		{0xE0, {0x0F, 0x1F, 0x1C, 0x0C, 0x0F, 0x08, 0x48, 0x98, 0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D, 0x00}, 15},
		{0XE1, {0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00}, 15},
		{0x20, {0}, 0},				/* display inversion OFF */
		{0x36, {0x48}, 1},
		{0x29, {0}, 0x80},			/* display on */
		{0x00, {0}, 0xff},
	};

	//Initialize GPIOs
    efHal_gpio_confPin(ILI9486_DC, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
    efHal_gpio_confPin(ILI9486_RST, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
    efHal_gpio_confPin(ILI9486_CS, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, 1);
    efHal_gpio_confBus(ILI9486_BUS, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE);

#ifdef ILI9486_RST
	//Reset the display
	efHal_gpio_setPin(ILI9486_RST, 0);
	vTaskDelay(100 / portTICK_RATE_MS);
	efHal_gpio_setPin(ILI9486_RST, 1);
	vTaskDelay(100 / portTICK_RATE_MS);
#endif

	//Send all the commands
	uint16_t cmd = 0;
	while (ili_init_cmds[cmd].databytes!=0xff) {
		ili9486_send_cmd(ili_init_cmds[cmd].cmd);
		ili9486_send_data(ili_init_cmds[cmd].data, ili_init_cmds[cmd].databytes&0x1F);
		if (ili_init_cmds[cmd].databytes & 0x80) {
			vTaskDelay(100 / portTICK_RATE_MS);
		}
		cmd++;
	}

    ili9486_set_orientation(CONFIG_LV_DISPLAY_ORIENTATION);
}

void ili9486_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
    uint8_t data[4] = {0};
    uint32_t size = 0;

    /*Column addresses*/
    ili9486_send_cmd(0x2A);
    data[0] = (area->x1 >> 8) & 0xFF;
    data[1] = area->x1 & 0xFF;
    data[2] = (area->x2 >> 8) & 0xFF;
    data[3] = area->x2 & 0xFF;
    ili9486_send_data(data, 4);

    /*Page addresses*/
    ili9486_send_cmd(0x2B);
    data[0] = (area->y1 >> 8) & 0xFF;
    data[1] = area->y1 & 0xFF;
    data[2] = (area->y2 >> 8) & 0xFF;
    data[3] = area->y2 & 0xFF;
    ili9486_send_data(data, 4);

    /*Memory write*/
    ili9486_send_cmd(0x2C);

    size = lv_area_get_width(area) * lv_area_get_height(area);

    ili9486_send_data((void*) color_map, size * 2);

    drv->draw_buf->flushing = 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void ili9486_send_cmd(uint8_t cmd)
{
	//disp_wait_for_pending_transactions();
    efHal_gpio_setPin(ILI9486_DC, 0);    /*Command mode*/
    efHal_gpio_setPin(ILI9486_CS, 0);
	efHal_gpio_writeBus(ILI9486_BUS, &cmd, 1);
	efHal_gpio_setPin(ILI9486_CS, 1);
}

static void ili9486_send_data(void * data, uint32_t length)
{
	//disp_wait_for_pending_transactions();
    efHal_gpio_setPin(ILI9486_DC, 1);    /*Data mode*/
    efHal_gpio_setPin(ILI9486_CS, 0);
	efHal_gpio_writeBus(ILI9486_BUS, data, length);
    efHal_gpio_setPin(ILI9486_CS, 1);
}

static void ili9486_set_orientation(uint8_t orientation)
{
    uint8_t data[] = {0x48, 0x88, 0x28, 0xE8};

    ili9486_send_cmd(0x36);
    ili9486_send_data((void *) &data[orientation], 1);
}
