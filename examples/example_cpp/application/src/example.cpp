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
#include "efHal_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Triangle.h"
#include "Square.h"
#include "appBoard.h"

/*==================[macros and typedef]=====================================*/
#define TRIANGLE_BASE_SIZE      10
#define TRIANGLE_HEIGHT_SIZE    4
#define SQUARE_BASE_SIZE        3
#define SQUARE_HEIGHT_SIZE      6

#define TOTAL_SHAPES            2

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static Triangle triangle(TRIANGLE_BASE_SIZE, TRIANGLE_HEIGHT_SIZE);
static Square square(SQUARE_BASE_SIZE, SQUARE_HEIGHT_SIZE);

static Shape *shapes[TOTAL_SHAPES] =
{
    &triangle,
    &square,
};

static float areas[TOTAL_SHAPES];

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void blinky_task(void *pvParameters)
{
    int i;

    for (i = 0 ; i < TOTAL_SHAPES ; i++)
    {
        if (shapes[i] != NULL)
        {
            areas[i] = shapes[i]->area();
            PRINT_DEBUG("Shape %d, area: %f\n", i+1, areas[i]);
        }
    }

    vTaskDelete(NULL);
}

/*==================[external functions definition]==========================*/
extern "C" void app_init(void)
{
    xTaskCreate(blinky_task, "blinky_task", 100, NULL, 0, NULL);
}

extern "C" void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (1);
}

extern "C" void vApplicationMallocFailedHook( void )
{
    while (1);
}


/*==================[end of file]============================================*/
