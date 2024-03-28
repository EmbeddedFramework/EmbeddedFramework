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
static void readBus(efHal_gpio_busid_t id, void *pData, size_t length);

/*==================[internal data definition]===============================*/

static efHal_gpio_callBacks_t cb =
{
    .setPin = setPin,
    .togPin = togPin,
    .getPin = getPin,
    .confInt = confInt,
    .confPin = confPin,
    .confBus = confBus,
    .writeBus = writeBus,
    .readBus = readBus,
};

static efHal_gpio_id_t _id;
static bool _stateW;
static bool _stateR;
static efHal_gpio_intType_t _intType;
static efHal_gpio_dir_t _dir;
static efHal_gpio_pull_t _pull;
static bool _state;
static void *_pData;
static size_t _length;
static bool cbCalled;
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void setPin(efHal_gpio_id_t id, bool state)
{
    cbCalled = true;
    TEST_ASSERT_EQUAL(_id, id);
    TEST_ASSERT_EQUAL(_stateW, state);
}

static void togPin(efHal_gpio_id_t id)
{
    cbCalled = true;
    TEST_ASSERT_EQUAL(_id, id);
}

static bool getPin(efHal_gpio_id_t id)
{
    cbCalled = true;
    TEST_ASSERT_EQUAL(_id, id);
    return _stateR;
}

static void confInt(efHal_gpio_id_t id, efHal_gpio_intType_t intType)
{
    cbCalled = true;
    TEST_ASSERT_EQUAL(_id, id);
    TEST_ASSERT_EQUAL(_intType, intType);
}

static void confPin(efHal_gpio_id_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull, bool state)
{
    cbCalled = true;
    TEST_ASSERT_EQUAL(_id, id);
    TEST_ASSERT_EQUAL(_dir, dir);
    TEST_ASSERT_EQUAL(_pull, pull);
    TEST_ASSERT_EQUAL(_state, state);
}

static void confBus(efHal_gpio_busid_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull)
{
    cbCalled = true;
    TEST_ASSERT_EQUAL(_id, id);
    TEST_ASSERT_EQUAL(_dir, dir);
    TEST_ASSERT_EQUAL(_pull, pull);
}

static void writeBus(efHal_gpio_busid_t id, void *pData, size_t length)
{
    cbCalled = true;
    TEST_ASSERT_EQUAL(_id, id);
    TEST_ASSERT_EQUAL_PTR(_pData, pData);
    TEST_ASSERT_EQUAL(_length, length);
}

static void readBus(efHal_gpio_busid_t id, void *pData, size_t length)
{
    cbCalled = true;
    TEST_ASSERT_EQUAL(_id, id);
    TEST_ASSERT_EQUAL_PTR(_pData, pData);
    TEST_ASSERT_EQUAL(_length, length);
}

/*==================[external functions definition]==========================*/

/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void) {
    cbCalled = false;
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
    _stateW = true;
    efHal_gpio_setPin(_id, _stateW);
    TEST_ASSERT_TRUE(cbCalled);
}

void test_efHal_gpio_togglePin_01(void)
{
    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);
    _id = 1234;
    efHal_gpio_togglePin(_id);
    TEST_ASSERT_TRUE(cbCalled);
}

void test_efHal_gpio_togglePin_02(void)
{
    vPortEnterCritical_Expect();
    vPortExitCritical_Expect();

    efHal_gpio_init();
    cb.togPin = NULL;
    efHal_internal_gpio_setCallBacks(cb);

    _id = 1234;
    _stateR = false;
    _stateW = true;

    efHal_gpio_togglePin(_id);
    TEST_ASSERT_TRUE(cbCalled);
}

void test_efHal_gpio_getPin_01(void)
{
    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);
    _id = 1234;
    _stateR = true;
    TEST_ASSERT_EQUAL(_stateR, efHal_gpio_getPin(_id));
    TEST_ASSERT_TRUE(cbCalled);
}

void test_efHal_gpio_confInt_01(void)
{
    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);
    _id = 1234;
    _intType = EF_HAL_GPIO_INT_TYPE_DISABLE;
    efHal_gpio_confInt(_id, _intType);
    TEST_ASSERT_TRUE(cbCalled);
}

void test_efHal_gpio_confInt_02(void)
{
    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);
    _id = 1234;
    _intType = EF_HAL_GPIO_INT_TYPE_RISING_EDGE;
    efHal_gpio_confInt(_id, _intType);
    TEST_ASSERT_TRUE(cbCalled);
}

void test_efHal_gpio_confPin_01(void)
{
    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);
    _id = 1234;
    _dir = EF_HAL_GPIO_OUTPUT;
    _pull = EF_HAL_GPIO_PULL_UP;
    _state = true;
    efHal_gpio_confPin(_id, _dir, _pull, _state);
    TEST_ASSERT_TRUE(cbCalled);
}

void test_efHal_gpio_waitForInt_01(void)
{
    static const TickType_t xTicksToWait = portMAX_DELAY;
    bool ret;

    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);

    xTaskGetCurrentTaskHandle_ExpectAndReturn((TaskHandle_t)0x1234);
    xTaskNotifyStateClear_ExpectAndReturn((TaskHandle_t)0x1234, 0);
    ulTaskNotifyTake_ExpectAndReturn(pdTRUE, xTicksToWait, 1);

    ret = efHal_gpio_waitForInt(0, xTicksToWait);

    TEST_ASSERT_EQUAL(true, ret);
}

void test_efHal_gpio_waitForInt_02(void)
{
    static const TickType_t xTicksToWait = 0;
    bool ret;

    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);

    xTaskGetCurrentTaskHandle_ExpectAndReturn((TaskHandle_t)0x1234);
    xTaskNotifyStateClear_ExpectAndReturn((TaskHandle_t)0x1234, 0);
    ulTaskNotifyTake_ExpectAndReturn(pdTRUE, xTicksToWait, 0);

    ret = efHal_gpio_waitForInt(0, xTicksToWait);

    TEST_ASSERT_EQUAL(false, ret);
}

void test_efHal_gpio_confBus_01(void)
{
    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);
    _id = 1234;
    _dir = EF_HAL_GPIO_OUTPUT;
    _pull = EF_HAL_GPIO_PULL_UP;
    efHal_gpio_confBus(_id, _dir, _pull);
    TEST_ASSERT_TRUE(cbCalled);
}

void test_efHal_gpio_writeBus_01(void)
{
    uint8_t testData[] = {0x01, 0x02, 0x03};

    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);
    _id = 0x1234;
    _pData = testData;
    _length = sizeof(testData);
    efHal_gpio_writeBus(_id, _pData, _length);
    TEST_ASSERT_TRUE(cbCalled);
}

void test_efHal_gpio_readBus_01(void)
{
    uint8_t testData[3];

    efHal_gpio_init();
    efHal_internal_gpio_setCallBacks(cb);
    _id = 0x1234;
    _pData = testData;
    _length = sizeof(testData);
    efHal_gpio_readBus(_id, _pData, _length);
    TEST_ASSERT_TRUE(cbCalled);
}

/*==================[end of file]============================================*/

//#endif // TEST
