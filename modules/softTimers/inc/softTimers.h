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

#ifndef SOFTTIMER_H_
#define SOFTTIMER_H_

/*==================[inclusions]=============================================*/
#include "stdint.h"
#include "stdbool.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[typedef]================================================*/

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/** \brief  Init Timers module
 **
 ** \return
 **/
extern void softTimers_init(void);

/** \brief  Open Timer
 ** init timer with clock selected
 **
 ** \param[in] clock selected
 **
 ** \return timer handler
 **/
extern int32_t softTimers_open(uint32_t baseTimeuS);

/** \brief  Close Timer
 ** deinit timer
 **
 ** \param[in] handler
 **
 ** \return
 **/
extern void softTimers_close(int32_t hTimer);

/** \brief  Get value of timer and clear
 ** return current value of timer and clear it
 **
 ** \param[in] handler
 **
 ** \return curren value of timer
 **/
extern uint32_t softTimers_getAndClear(int32_t hTimer);


/** \brief  Get value of timer
 ** return current value of timer
 **
 ** \param[in] handler
 ** \param[in] clear set true to clear elapsed time
 **
 ** \return curren value of timer
 **/
extern uint32_t softTimers_get(int32_t hTimer, bool clear);

/** \brief  Clearvalue of timer
 **
 ** \param[in] handler
 **
 ** \return
 **/
extern void softTimers_clear(int32_t hTimer);

/** \brief  timer rollover
 **
 ** \remark: this function must be called from timer's ISR
 **/
extern void softTimers_rollOver(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* SOFTTIMER_H_ */
