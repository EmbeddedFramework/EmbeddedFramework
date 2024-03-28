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
#include "mock_semphr.h"
#include "mock_queue.h"

#include "efHal_uart.h"
#include "efHal_internal.h"

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

/*==================[macros and typedef]=====================================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static efHal_dh_t efHal_dh_UART;
static void* _param;
static const efHal_uart_conf_t _uart_conf =
{
        .baudrate = 57600,
};
static bool cbCalled;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void confCB(void *param, efHal_uart_conf_t const *cfg)
{
    TEST_ASSERT_EQUAL(_param, param);
    TEST_ASSERT_EQUAL_MEMORY(&_uart_conf, cfg, sizeof(efHal_uart_conf_t));
    cbCalled = true;
}

static void dataReadyTx(void *param)
{

}

/*==================[external functions definition]==========================*/


/** \brief set Up function
 **
 ** This function is called before each test case is executed
 **
 **/
void setUp(void) {

    efHal_uart_callBacks_t cb;

    cb.conf = confCB;
    cb.dataReadyTx = dataReadyTx;
    cb.sendBuffer = NULL;
    _param = (void*)0x1234;

    vPortEnterCritical_Ignore();
    vPortExitCritical_Ignore();
    xQueueCreateMutex_ExpectAndReturn(queueQUEUE_TYPE_MUTEX, (QueueHandle_t)0x1000);
    xQueueGenericCreate_ExpectAndReturn(16,sizeof(uint8_t),queueQUEUE_TYPE_BASE, (QueueHandle_t)0x2000);
    xQueueGenericCreate_ExpectAndReturn(16,sizeof(uint8_t),queueQUEUE_TYPE_BASE, (QueueHandle_t)0x2001);

    efHal_uart_init();

    efHal_dh_UART = efHal_internal_uart_deviceReg(cb, _param);

    cbCalled = 0;
}

/** \brief tear Down function
 **
 ** This function is called after each test case is executed
 **
 **/
void tearDown(void) {
}

void test_efHal_uart_conf(void)
{
    efHal_uart_conf(efHal_dh_UART, &_uart_conf);
    TEST_ASSERT_TRUE(cbCalled);
}

/*==================[support functions]============================================*/

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

//#endif // TEST
