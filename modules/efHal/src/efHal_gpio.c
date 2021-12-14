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
#include "efHal_gpio.h"
#include "efHal_internal.h"

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static efHal_gpio_callBacks_t callBacks;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void efHal_gpio_init(void)
{

}

extern void efHal_gpio_setPin(efHal_gpio_id_t id, bool state)
{
    if (callBacks.setPin != NULL)
        callBacks.setPin(id, state);
    else
    {
        /* TODO ASSERT */
    }
}

extern void efHal_gpio_togglePin(efHal_gpio_id_t id)
{
    if (callBacks.togPin != NULL)
        callBacks.togPin(id);
    else
    {
        bool state;
        vPortEnterCritical();
        state = efHal_gpio_getPin(id);
        efHal_gpio_setPin(id, !state);
        vPortExitCritical();
    }
}

extern bool efHal_gpio_getPin(efHal_gpio_id_t id)
{
    bool ret = 0;

    if (callBacks.getPin != NULL)
        ret = callBacks.getPin(id);
    else
    {
        /* TODO ASSERT */
    }

    return ret;
}

extern void efHal_internal_gpio_setCallBacks(efHal_gpio_callBacks_t cb)
{
    callBacks = cb;
}


/*==================[end of file]============================================*/
