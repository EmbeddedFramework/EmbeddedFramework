/*
###############################################################################
#
# Copyright 2021, Gustavo Muro
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
#include "efHal_i2c.h"
#include "efHal_spi.h"
#include "efHal_analog.h"
#include "efHal_internal.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void efHal_init(void)
{
    efHal_gpio_init();

    efHal_analog_init();

    efHal_pwm_init();

#if EF_HAL_I2C_TOTAL_DEVICES > 0
    efHal_i2c_init();
#endif
#if EF_HAL_UART_TOTAL_DEVICES > 0
    efHal_uart_init();
#endif

#if EF_HAL_SPI_TOTAL_DEVICES > 0
    efHal_spi_init();
#endif

}

extern efHal_dh_t efHal_internal_searchFreeSlot(efHal_internal_dhD_t *p_dhD, size_t size, size_t length)
{
    efHal_dh_t ret = NULL;
    uint8_t *pTemp = (uint8_t*)p_dhD;
    int i;

    for (i = 0 ; i < length ; i++)
    {
        if (p_dhD->mutex == NULL)
        {
            ret = p_dhD;
            break;
        }

        pTemp += size;
        p_dhD = (efHal_internal_dhD_t*)pTemp;
    }

    return ret;
}


/*==================[end of file]============================================*/
