/*
###############################################################################
#
# Copyright 2024, Gustavo Muro
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

//#ifdef TEST

/*==================[inclusions]=============================================*/

#include "unity.h"
#include "cmock.h"

#include "FreeRTOS.h"
#include "mock_FreeRTOS.h"
#include "mock_portmacro.h"
#include "mock_task.h"

#include "efHal_gpio.h"
#include "efHal_internal.h"


/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

static void setPin(efHal_gpio_id_t id, bool state);
static void togPin(efHal_gpio_id_t id);
static bool getPin(efHal_gpio_id_t id);
static void confInt(efHal_gpio_id_t id, efHal_gpio_intType_t intType);
static void confPin(efHal_gpio_id_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull, bool state);
static void confBus(efHal_gpio_busid_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull);
static void writeBus(efHal_gpio_busid_t id, void *pData, size_t length);

/*==================[internal data definition]===============================*/

static const efHal_gpio_callBacks_t cb =
{
    .setPin = setPin,
    .togPin = togPin,
};

static efHal_gpio_id_t _id;
static bool _state;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void setPin(efHal_gpio_id_t id, bool state)
{
    TEST_ASSERT_EQUAL(_id, id);
    TEST_ASSERT_EQUAL(_state, state);
}

static void togPin(efHal_gpio_id_t id)
{
    TEST_ASSERT_EQUAL(_id, id);
}

/*==================[external functions definition]==========================*/

/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void) {
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {
}

void test_efHal_gpio_setPin_01(void)
{
    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);
    _id = 1234;
    _state = true;
    efHal_gpio_setPin(_id, _state);
}

void test_efHal_gpio_togglePin_01(void)
{
    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);
    _id = 1234;
    efHal_gpio_togglePin(_id);
}

/*==================[end of file]============================================*/

//#endif // TEST
