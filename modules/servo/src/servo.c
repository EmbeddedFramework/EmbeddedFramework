/*
###############################################################################
#
# Copyright 2023, Guido Cicconi
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
#include "servo.h"
#include "FreeRTOS.h"
#include "task.h"

/*==================[macros and typedef]=====================================*/

#define SERVO_PWM_FREQ 50

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

efHal_pwm_id_t _pwmPin;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void servo_init(efHal_pwm_id_t pwmPin, uint8_t initialPos){
    _pwmPin = pwmPin;
    efHal_pwm_setPeriod(pwmPin, (uint32_t)((float)1e9/(float)SERVO_PWM_FREQ));
    servo_setPos(initialPos);
    vTaskDelay(pdMS_TO_TICKS(1000));
    return;
}

extern void servo_setPos(uint8_t posDegree){
    float servoDutyCount = 0;

    servoDutyCount = SERVO_PWM_MIN_DUTY_US*1e-6*50.0*(efHal_pwm_getPeriodCount(_pwmPin) + 1) +
                     (float)posDegree/180.0 * ((SERVO_PWM_MAX_DUTY_US*1e-6*50.0*(efHal_pwm_getPeriodCount(_pwmPin) + 1))-
                     (SERVO_PWM_MIN_DUTY_US*1e-6*50.0*(efHal_pwm_getPeriodCount(_pwmPin) + 1)));

    efHal_pwm_setDuty(_pwmPin, (uint32_t)servoDutyCount, EF_HAL_PWM_DUTY_COUNT);

    return;
}

/*==================[end of file]============================================*/





