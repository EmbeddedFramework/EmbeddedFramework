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
#include <array>

/*==================[macros and typedef]=====================================*/
constexpr float TRIANGLE_BASE_SIZE = 10;
constexpr float TRIANGLE_HEIGHT_SIZE = 4;
constexpr float SQUARE_SIDE_SIZE = 3;

constexpr int TOTAL_SHAPES = 2;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static const Triangle triangle(TRIANGLE_BASE_SIZE, TRIANGLE_HEIGHT_SIZE);
static const Square square(SQUARE_SIDE_SIZE);

static std::array<const Shape*, TOTAL_SHAPES> shapes = {&triangle, &square};

static std::array<float, TOTAL_SHAPES> areas;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
static void blinky_task(void *pvParameters)
{
    for (size_t i = 0 ; i < shapes.size() ; i++)
    {
        if (shapes[i] != nullptr)
        {
            areas[i] = shapes[i]->area();
            PRINT_DEBUG("Shape %zu, area: %f\n", i+1, areas[i]);
        }
    }

    vTaskDelete(nullptr);
}

/*==================[external functions definition]==========================*/
extern "C" void app_init(void)
{
    xTaskCreate(blinky_task, "blinky_task", 100, nullptr, 0, nullptr);
}

extern "C" __attribute__((noreturn)) void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while (true);
}

extern "C" __attribute__((noreturn)) void vApplicationMallocFailedHook( void )
{
    while (true);
}


/*==================[end of file]============================================*/
