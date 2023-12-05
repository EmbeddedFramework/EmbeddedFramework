/*****************************************************************************
 *   oled.c:  Driver for SSD1306 OLED Display
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


/******************************************************************************
 * Includes
 *****************************************************************************/
#include <string.h>
#include "oled.h"
#include "font5x7.h"
#include "efHal_spi.h"

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define OLED_DATA()   efHal_gpio_setPin(_cmdPin, true)
#define OLED_CMD()    efHal_gpio_setPin(_cmdPin, false)

#define OLED_ENABLE() efHal_gpio_setPin(_rstPin, true)
#define OLED_DISABLE() efHal_gpio_setPin(_rstPin, false)

#define X_OFFSET 0

#define SHADOW_FB_SIZE (OLED_DISPLAY_WIDTH*OLED_DISPLAY_HEIGHT >> 3)

#define setAddress(page,lowerAddr,higherAddr)\
    writeCommand(page);\
    writeCommand(lowerAddr);\
    writeCommand(higherAddr);

/******************************************************************************
 * External global variables
 *****************************************************************************/


/******************************************************************************
 * Local variables
 *****************************************************************************/
efHal_dh_t _spi;
efHal_gpio_id_t _cmdPin, _rstPin;

/*
 * The SSD1306 doesn't support reading from the display memory when using
 * serial mode (only parallel mode). Since it isn't possible to write only
 * one pixel to the display (a minimum of one column, 8 pixels, is always
 * writen) a shadow framebuffer is needed to keep track of the display
 * data.
 */
static uint8_t shadowFB[SHADOW_FB_SIZE];

static uint8_t const  font_mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};


/******************************************************************************
 * Local Functions
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *    Write a command to the display
 *
 * Params:
 *   [in] data - command to write to the display
 *
 *****************************************************************************/
static void
writeCommand(uint8_t data)
{
	OLED_CMD();
	efHal_spi_transfer(_spi, &data, NULL, sizeof(data));
}

/******************************************************************************
 *
 * Description:
 *    Write data to the display
 *
 * Params:
 *   [in] data - data (color) to write to the display
 *
 *****************************************************************************/
static void
writeData(uint8_t data)
{
    OLED_DATA();
    efHal_spi_transfer(_spi, &data, NULL, sizeof(data));
}

/******************************************************************************
 *
 * Description:
 *    Write len number of data to the display
 *
 * Params:
 *   [in] data - data (color) to write to the display
 *   [in] len  - number of bytes to write
 *
 *****************************************************************************/
static void
writeDataLen(unsigned char data, unsigned int len)
{
	int i;
    uint8_t buf[140];

    for (i = 0; i < len; i++) {
        buf[i] = data;
    }

    OLED_DATA();

    efHal_spi_transfer(_spi, buf, NULL, len);
}


/******************************************************************************
 *
 * Description:
 *    Run display init sequence
 *
 *****************************************************************************/
static void
runInitSequence()
{
    /*
     * Values extracted from Adafruit library (https://github.com/adafruit/Adafruit_SSD1306)
     */

	 writeCommand(0xAE);
	 writeCommand(0xD5);
	 writeCommand(0x80);
	 writeCommand(0xA8);
	 writeCommand((uint8_t)(OLED_DISPLAY_HEIGHT-1));
	 writeCommand(0xD3);
	 writeCommand(0x00);
	 writeCommand(0x40);
	 writeCommand(0x8D);
	 writeCommand(0x14);
	 writeCommand(0x20);
	 writeCommand(0x00);
	 writeCommand(0xA1);
	 writeCommand(0xC8);
	 writeCommand(0xDA);
	 writeCommand(0x12);
	 writeCommand(0x81);
	 writeCommand(0x7F);
	 writeCommand(0xD9);
	 writeCommand(0xF1);
	 writeCommand(0xDB);
	 writeCommand(0x40);
	 writeCommand(0xA4);
	 writeCommand(0xA6);
	 writeCommand(0x2E);
	 writeCommand(0xAF);
}


/******************************************************************************
 *
 * Description:
 *    Draw a horizontal line
 *
 * Params:
 *   [in] x0 - start x position
 *   [in] y0 - y position
 *   [in] x1 - end x position
 *   [in] color - color of the pixel
 *
 *****************************************************************************/
static void hLine(uint8_t x0, uint8_t y0, uint8_t x1, oled_color_t color)
{
    uint8_t bak;

    if (x0 > x1)
    {
        bak = x1;
        x1 = x0;
        x0 = bak;
    }

    while(x1 >= x0)
    {
        oled_putPixel(x0, y0, color);
        x0++;
    }
}

/******************************************************************************
 *
 * Description:
 *    Draw a vertical line
 *
 * Params:
 *   [in] x0 - x position
 *   [in] y0 - start y position
 *   [in] y1 - end y position
 *   [in] color - color of the pixel
 *
 *****************************************************************************/
static void vLine(uint8_t x0, uint8_t y0, uint8_t y1, oled_color_t color)
{
    uint8_t bak;

    if(y0 > y1)
    {
        bak = y1;
        y1 = y0;
        y0 = bak;
    }

    while(y1 >= y0)
    {
        oled_putPixel(x0, y0, color);
        y0++;
    }
    return;
}


/******************************************************************************
 * Public Functions
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *    Initialize the OLED Display
 *
 *****************************************************************************/
void oled_init(efHal_dh_t spi, efHal_gpio_id_t cmdPin, efHal_gpio_id_t rstPin)
{
    int i = 0;

    _cmdPin = cmdPin;
    _rstPin = rstPin;
    _spi = spi;

    efHal_spi_config(spi, 4000000, EF_HAL_SPI_CPOL_1_CPHA_1);

    /* make sure power is off */
    OLED_DISABLE();

    /* small delay before turning on power */
    for (i = 0; i < 0xffff; i++);

    /* power on */
    OLED_ENABLE();

    runInitSequence();

    memset(shadowFB, 0, SHADOW_FB_SIZE);
}

/******************************************************************************
 *
 * Description:
 *    Initialize the OLED Display
 *
 * Params:
 * 	 [in] contrast - display contrast (0-255)
 *****************************************************************************/

void oled_setContrast(uint8_t contrast){
	writeCommand(0x81);
	writeCommand(contrast);
}

/******************************************************************************
 *
 * Description:
 *    Draw one pixel on the display
 *
 * Params:
 *   [in] x - x position
 *   [in] y - y position
 *   [in] color - color of the pixel
 *
 *****************************************************************************/
void oled_putPixel(uint8_t x, uint8_t y, oled_color_t color) {
    uint8_t page;
    uint16_t add;
    uint8_t lAddr;
    uint8_t hAddr;
    uint8_t mask;
    uint32_t shadowPos = 0;

    if (x > OLED_DISPLAY_WIDTH) {
        return;
    }
    if (y > OLED_DISPLAY_HEIGHT) {
        return;
    }

    /* page address */
         if(y < 8)  page = 0xB0;
    else if(y < 16) page = 0xB1;
    else if(y < 24) page = 0xB2;
    else if(y < 32) page = 0xB3;
    else if(y < 40) page = 0xB4;
    else if(y < 48) page = 0xB5;
    else if(y < 56) page = 0xB6;
    else            page = 0xB7;

    add = x + X_OFFSET;
    lAddr = 0x0F & add;             // Low address
    hAddr = 0x10 | (add >> 4);      // High address

    // Calculate mask from rows basically do a y%8 and remainder is bit position
    add = y>>3;                     // Divide by 8
    add <<= 3;                      // Multiply by 8
    add = y - add;                  // Calculate bit position
    mask = 1 << add;                // Left shift 1 by bit position

    setAddress(page, lAddr, hAddr); // Set the address (sets the page,
                                    // lower and higher column address pointers)

    shadowPos = (page-0xB0)*OLED_DISPLAY_WIDTH+x;

    if(color > 0)
        shadowFB[shadowPos] |= mask;
    else
        shadowFB[shadowPos] &= ~mask;

    writeData(shadowFB[shadowPos]);
}

/******************************************************************************
 *
 * Description:
 *    Draw a line on the display starting at x0,y0 and ending at x1,y1
 *
 * Params:
 *   [in] x0 - start x position
 *   [in] y0 - start y position
 *   [in] x1 - end x position
 *   [in] y1 - end y position
 *   [in] color - color of the line
 *
 *****************************************************************************/
void oled_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, oled_color_t color)
{
    int16_t   dx = 0, dy = 0;
    int8_t    dx_sym = 0, dy_sym = 0;
    int16_t   dx_x2 = 0, dy_x2 = 0;
    int16_t   di = 0;

    dx = x1-x0;
    dy = y1-y0;


    if(dx == 0)           /* vertical line */
    {
        vLine(x0, y0, y1, color);
        return;
    }

    if(dx > 0)
    {
        dx_sym = 1;
    }
    else
    {
        dx_sym = -1;
    }


    if(dy == 0)           /* horizontal line */
    {
        hLine(x0, y0, x1, color);
        return;
    }


    if(dy > 0)
    {
        dy_sym = 1;
    }
    else
    {
        dy_sym = -1;
    }

    dx = dx_sym*dx;
    dy = dy_sym*dy;

    dx_x2 = dx*2;
    dy_x2 = dy*2;

    if(dx >= dy)
    {
        di = dy_x2 - dx;
        while(x0 != x1)
        {

            oled_putPixel(x0, y0, color);
            x0 += dx_sym;
            if(di<0)
            {
                di += dy_x2;
            }
            else
            {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        oled_putPixel(x0, y0, color);
    }
    else
    {
        di = dx_x2 - dy;
        while(y0 != y1)
        {
            oled_putPixel(x0, y0, color);
            y0 += dy_sym;
            if(di < 0)
            {
                di += dx_x2;
            }
            else
            {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        oled_putPixel(x0, y0, color);
    }
    return;
}

/******************************************************************************
 *
 * Description:
 *    Draw a circle on the display starting at x0,y0 with radius r
 *
 * Params:
 *   [in] x0 - start x position
 *   [in] y0 - start y position
 *   [in] r - radius
 *   [in] color - color of the circle
 *
 *****************************************************************************/
void oled_circle(uint8_t x0, uint8_t y0, uint8_t r, oled_color_t color)
{
    int16_t draw_x0, draw_y0;
    int16_t draw_x1, draw_y1;
    int16_t draw_x2, draw_y2;
    int16_t draw_x3, draw_y3;
    int16_t draw_x4, draw_y4;
    int16_t draw_x5, draw_y5;
    int16_t draw_x6, draw_y6;
    int16_t draw_x7, draw_y7;
    int16_t xx, yy;
    int16_t  di;

    if(r == 0)          /* no radius */
    {
        return;
    }

    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
    if(draw_y0 < OLED_DISPLAY_HEIGHT)
    {
        oled_putPixel(draw_x0, draw_y0, color);     /* 90 degree */
    }

    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
    if(draw_y2 >= 0)
    {
        oled_putPixel(draw_x2, draw_y2, color);    /* 270 degree */
    }

    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
    if(draw_x4 < OLED_DISPLAY_WIDTH)
    {
        oled_putPixel(draw_x4, draw_y4, color);     /* 0 degree */
    }

    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
    if(draw_x5>=0)
    {
        oled_putPixel(draw_x5, draw_y5, color);     /* 180 degree */
    }

    if(r == 1)
    {
        return;
    }

    di = 3 - 2*r;
    xx = 0;
    yy = r;
    while(xx < yy)
    {

        if(di < 0)
        {
            di += 4*xx + 6;
        }
        else
        {
            di += 4*(xx - yy) + 10;
            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
            draw_x4--;
            draw_x5++;
            draw_x6--;
            draw_x7++;
        }
        xx++;
        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;
        draw_y4++;
        draw_y5++;
        draw_y6--;
        draw_y7--;

        if( (draw_x0 <= OLED_DISPLAY_WIDTH) && (draw_y0>=0) )
        {
            oled_putPixel(draw_x0, draw_y0, color);
        }

        if( (draw_x1 >= 0) && (draw_y1 >= 0) )
        {
            oled_putPixel(draw_x1, draw_y1, color);
        }

        if( (draw_x2 <= OLED_DISPLAY_WIDTH) && (draw_y2 <= OLED_DISPLAY_HEIGHT) )
        {
            oled_putPixel(draw_x2, draw_y2, color);
        }

        if( (draw_x3 >=0 ) && (draw_y3 <= OLED_DISPLAY_HEIGHT) )
        {
            oled_putPixel(draw_x3, draw_y3, color);
        }

        if( (draw_x4 <= /*OLED_DISPLAY_HEIGHT*/OLED_DISPLAY_WIDTH) && (draw_y4 >= 0) )
        {
            oled_putPixel(draw_x4, draw_y4, color);
        }

        if( (draw_x5 >= 0) && (draw_y5 >= 0) )
        {
            oled_putPixel(draw_x5, draw_y5, color);
        }
        if( (draw_x6 <= OLED_DISPLAY_WIDTH) && (draw_y6 <= OLED_DISPLAY_HEIGHT) )
        {
            oled_putPixel(draw_x6, draw_y6, color);
        }
        if( (draw_x7 >= 0) && (draw_y7 <= OLED_DISPLAY_HEIGHT) )
        {
            oled_putPixel(draw_x7, draw_y7, color);
        }
    }
    return;
}

/******************************************************************************
 *
 * Description:
 *    Draw a rectangle on the display starting at x0,y0 and ending at x1,y1
 *
 * Params:
 *   [in] x0 - start x position
 *   [in] y0 - start y position
 *   [in] x1 - end x position
 *   [in] y1 - end y position
 *   [in] color - color of the rectangle
 *
 *****************************************************************************/
void oled_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, oled_color_t color)
{
    hLine(x0, y0, x1, color);
    hLine(x0, y1, x1, color);
    vLine(x0, y0, y1, color);
    vLine(x1, y0, y1, color);
}

/******************************************************************************
 *
 * Description:
 *    Fill a rectangle on the display starting at x0,y0 and ending at x1,y1
 *
 * Params:
 *   [in] x0 - start x position
 *   [in] y0 - start y position
 *   [in] x1 - end x position
 *   [in] y1 - end y position
 *   [in] color - color of the rectangle
 *
 *****************************************************************************/
void oled_fillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, oled_color_t color)
{
    uint8_t i = 0;

    if(x0 > x1)
    {
        i  = x0;
        x0 = x1;
        x1 = i;
    }

    if(y0 > y1)
    {
        i  = y0;
        y0 = y1;
        y1 = i;
    }

    if(y0 == y1)
    {
        hLine(x0, y0, x1, color);
        return;
    }

    if(x0 == x1)
    {
        vLine(x0, y0, y1, color);
        return;
    }

    while(y0 <= y1)
    {
        hLine(x0, y0, x1, color);
        y0++;
    }
    return;
}

/******************************************************************************
 *
 * Description:
 *    Clear the entire screen
 *
 * Params:
 *   [in] color - color to fill the screen with
 *
 *****************************************************************************/
void oled_clearScreen(oled_color_t color)
{
    uint8_t i;
    uint8_t c = 0;

    if (color == OLED_COLOR_WHITE)
        c = 0xff;


    for(i=0xB0;i<0xB8;i++) {            // Go through all 8 pages
        setAddress(i,0x00,0x10);
        writeDataLen(c, 132);
    }

    memset(shadowFB, c, SHADOW_FB_SIZE);
}

uint8_t oled_putChar(uint8_t x, uint8_t y, uint8_t ch, oled_color_t fb, oled_color_t bg)
{
    unsigned char data = 0;
    unsigned char i = 0, j = 0;

    oled_color_t color = OLED_COLOR_BLACK;

    if((x >= (OLED_DISPLAY_WIDTH - 8)) || (y >= (OLED_DISPLAY_HEIGHT - 8)) )
    {
        return 0;
    }

    if( (ch < 0x20) || (ch > 0x7f) )
    {
        ch = 0x20;      /* unknown character will be set to blank */
    }

    ch -= 0x20;
    for(i=0; i<8; i++)
    {
        data = font5x7[ch][i];
        for(j=0; j<6; j++)
        {
            if( (data&font_mask[j])==0 )
            {
                color = bg;
            }
            else
            {
                color = fb;
            }
            oled_putPixel(x, y, color);
            x++;
        }
        y++;
        x -= 6;
    }
    return( 1 );
}

void oled_putString(uint8_t x, uint8_t y, uint8_t *pStr, oled_color_t fb,
        oled_color_t bg)
{
  while(1)
  {
      if( (*pStr)=='\0' )
      {
          break;
      }
      if( oled_putChar(x, y, *pStr++, fb, bg) == 0 )
      {
        break;
    }
    x += 6;
  }
  return;
}

