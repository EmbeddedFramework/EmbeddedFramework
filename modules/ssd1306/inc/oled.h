/*****************************************************************************
 *   oled.h:  Header file for SSD1306 OLED Display
 *
 *   Copyright 2009, Embedded Artists AB
 *   Copyright 2023, DSI FCEIA UNR - Sistemas Digitales 2
 *    DSI: http://www.dsi.fceia.unr.edu.ar/
 *   Copyright 2023, Guido Cicconi
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef __OLED_H
#define __OLED_H

/*==================[inclusions]=============================================*/
#include <stdint.h>
#include "efHal.h"
#include "efHal_gpio.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/
typedef enum
{
    OLED_COLOR_BLACK,
    OLED_COLOR_WHITE
} oled_color_t;

#define OLED_DISPLAY_WIDTH  128
#define OLED_DISPLAY_HEIGHT 64

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions definition]==========================*/

void oled_init(efHal_dh_t spi, efHal_gpio_id_t cmdPin, efHal_gpio_id_t rstPin);
void oled_putPixel(uint8_t x, uint8_t y, oled_color_t color);
void oled_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, oled_color_t color);
void oled_circle(uint8_t x0, uint8_t y0, uint8_t r, oled_color_t color);
void oled_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, oled_color_t color);
void oled_fillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, oled_color_t color);
void oled_clearScreen(oled_color_t color);
void oled_putString(uint8_t x, uint8_t y, uint8_t *pStr, oled_color_t fb, oled_color_t bg);
uint8_t oled_putChar(uint8_t x, uint8_t y, uint8_t ch, oled_color_t fb, oled_color_t bg);
void oled_setContrast(uint8_t contrast);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

#endif /* end __OLED_H */
