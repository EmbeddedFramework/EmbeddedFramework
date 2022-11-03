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

#ifndef ILI9486_H
#define ILI9486_H

/*==================[inclusions]=============================================*/
#include "lvgl.h"
#include "efHal_gpio.h"

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/
#define DISPLAY_ORIENTATION_VERTICAL 0
#define DISPLAY_ORIENTATION_VERTICAL_INV 1
#define DISPLAY_ORIENTATION_HORIZONTAL 2
#define DISPLAY_ORIENTATION_HORIZONTAL_INV 3


/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

void ili9486_init(int ori, efHal_gpio_id_t dc, efHal_gpio_id_t rst, efHal_gpio_id_t cs, efHal_gpio_busid_t bus);
void ili9486_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);

/*==================[cplusplus]==============================================*/

#ifdef __cplusplus
} /* extern "C" */
#endif


/*==================[end of file]============================================*/
#endif /* ILI9486_H*/
