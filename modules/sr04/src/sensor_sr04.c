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
#include "sensor_sr04.h"
#include "task.h"
#include "softTimers.h"

#define TRIG_PIN_PULSE_US 10

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

int32_t timerHandler = 0;
efHal_gpio_id_t _trigPin, _echoPin;
sr04_trigger_callback _cb;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

extern void sensor_sr04_init(efHal_gpio_id_t trigPin, efHal_gpio_id_t echoPin, sr04_trigger_callback cb){
    _trigPin = trigPin;
    _echoPin = echoPin;
    _cb = cb;

    efHal_gpio_confPin(trigPin, EF_HAL_GPIO_OUTPUT, EF_HAL_GPIO_PULL_DISABLE, false);
    efHal_gpio_setPin(trigPin, false);
    efHal_gpio_confPin(echoPin, EF_HAL_GPIO_INPUT, EF_HAL_GPIO_PULL_DISABLE, false);
    efHal_gpio_confInt(echoPin, EF_HAL_GPIO_INT_TYPE_BOTH_EDGE);

    softTimers_init();
    timerHandler = softTimers_open(1);
}

extern uint16_t sensor_sr04_measure(sensor_distance_t unit){

    uint16_t distance;
    bool error = false;

    _cb(_trigPin, TRIG_PIN_PULSE_US);

    if(!efHal_gpio_waitForInt(_echoPin, pdMS_TO_TICKS(500))) error = true; //Wait for rising edge
    softTimers_clear(timerHandler);
    if(!efHal_gpio_waitForInt(_echoPin, pdMS_TO_TICKS(30))) error = true;  //Wait for falling edge

    if(unit == SENSOR_UNIT_CM)
        distance = softTimers_get(timerHandler, false)/58;
    else if (unit == SENSOR_UNIT_INCHES)
        distance = softTimers_get(timerHandler, false)/148;
    else
        distance = 0;

    if(error) distance = SENSOR_SR04_ERROR;

    return distance;
}

/*==================[end of file]============================================*/
