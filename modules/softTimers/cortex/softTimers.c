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
#include "softTimers.h"

#ifdef CPU_LPC51U68JBD48
#include "LPC51U68.h"
#endif

#ifdef CPU_MKL46Z256VLL4
#include "MKL46Z4.h"
#endif

#ifdef STM32H743xx
#include "stm32h743xx.h"
#endif

#if __has_include("softTimers_config.h")
    #include "softTimers_config.h"
#endif

/*==================[macros and definitions]=================================*/

#ifndef softTimers_TOTAL_TIMES
    #define softTimers_TOTAL_TIMES     (5)
#endif

typedef struct
{
   unsigned inUse:1;

   int32_t rollOver;
   int32_t baseTimeuS;
   int32_t lastValue;

}timer_vars_type;

/*==================[internal data declaration]==============================*/
static timer_vars_type timer_vars[softTimers_TOTAL_TIMES];
static int32_t systickClockMhz;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void softTimers_init(void)
{
   uint32_t loopi;

   for (loopi = 0 ; softTimers_TOTAL_TIMES > loopi ; loopi++)
   {
      timer_vars[loopi].inUse = 0;
   }

   systickClockMhz = SystemCoreClock / 1000000;
}

extern int32_t softTimers_open(uint32_t baseTimeuS)
{
   uint32_t loopi;
   int32_t ret = -1;

   if (0 < baseTimeuS)
   {
      for (loopi = 0 ; (softTimers_TOTAL_TIMES > loopi) && (-1 == ret) ; loopi++)
      {
         if (0 == timer_vars[loopi].inUse)
         {
            timer_vars[loopi].inUse = 1;
            timer_vars[loopi].baseTimeuS = baseTimeuS;
            timer_vars[loopi].rollOver = 0;
            timer_vars[loopi].lastValue = SysTick->VAL;
            ret = loopi;
         }
      }
   }

   return ret;
}

extern void softTimers_close(int32_t hTimer)
{
   timer_vars[hTimer].inUse = 0;
}

extern uint32_t softTimers_getAndClear(int32_t hTimer)
{
   return softTimers_get(hTimer, true);
}

extern uint32_t softTimers_get(int32_t hTimer, bool clear)
{
   int32_t elapsed;
   int32_t timeMs;
   int32_t timeSysTick;
   int32_t sysTickValue;
   int32_t sysTickLastValue;

   /* disable interrupt */
   SysTick->CTRL = SysTick->CTRL & ~(SysTick_CTRL_TICKINT_Msk);

   timeMs = timer_vars[hTimer].rollOver;
   sysTickValue = SysTick->VAL;
   sysTickLastValue = timer_vars[hTimer].lastValue;

   if (clear)
   {
       timer_vars[hTimer].rollOver = 0;
       timer_vars[hTimer].lastValue = SysTick->VAL;
   }

   /* enable interrupt */
   SysTick->CTRL = SysTick->CTRL | SysTick_CTRL_TICKINT_Msk;

   timeSysTick = (sysTickLastValue - sysTickValue) / systickClockMhz;

   elapsed = (timeMs * 1000 + timeSysTick) / timer_vars[hTimer].baseTimeuS;

   if (elapsed < 0)
   {
      elapsed = 0;
   }

   return elapsed;
}

extern void softTimers_clear(int32_t hTimer)
{
   /* disable interrupt */
   SysTick->CTRL = SysTick->CTRL & ~(SysTick_CTRL_TICKINT_Msk);

   timer_vars[hTimer].rollOver = 0;
   timer_vars[hTimer].lastValue = SysTick->VAL;

   /* enable interrupt */
   SysTick->CTRL = SysTick->CTRL | SysTick_CTRL_TICKINT_Msk;
}

extern void softTimers_rollOver(void)
{
   uint32_t loopi;

   for (loopi = 0 ; softTimers_TOTAL_TIMES > loopi ; loopi++)
   {
      if (timer_vars[loopi].inUse)
      {
         if (UINT32_MAX > timer_vars[loopi].rollOver)
         {
            timer_vars[loopi].rollOver++;
         }
      }
   }
}

/*==================[end of file]============================================*/
