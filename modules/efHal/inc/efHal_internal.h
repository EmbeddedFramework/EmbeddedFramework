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
#ifndef EF_HAL_INTERNAL_H_
#define EF_HAL_INTERNAL_H_

/*==================[inclusions]=============================================*/
#include "efHal.h"
#include "efHal_config.h"
#include "efHal_gpio.h"
#include "efHal_i2c.h"
#include "efHal_uart.h"
#include "FreeRTOS.h"
#include "semphr.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

typedef struct
{
    SemaphoreHandle_t mutex;
    TaskHandle_t taskHadle;
}efHal_internal_dhD_t;

/******************************* GPIO ****************************************/

typedef void (*efHal_gpio_setPin_t)(efHal_gpio_id_t id, bool state);
typedef void (*efHal_gpio_togPin_t)(efHal_gpio_id_t id);
typedef bool (*efHal_gpio_getPin_t)(efHal_gpio_id_t id);
typedef void (*efHal_gpio_confInt_t)(efHal_gpio_id_t id, efHal_gpio_intType_t intType);
typedef void (*efHal_gpio_confPin_t)(efHal_gpio_id_t id, efHal_gpio_dir_t dir, efHal_gpio_pull_t pull, bool state);

typedef struct
{
    efHal_gpio_setPin_t setPin;
    efHal_gpio_togPin_t togPin;
    efHal_gpio_getPin_t getPin;
    efHal_gpio_confInt_t confInt;
    efHal_gpio_confPin_t confPin;
}efHal_gpio_callBacks_t;

#ifndef EF_HAL_GPIO_TOTAL_CALL_BACK
#define EF_HAL_GPIO_TOTAL_CALL_BACK 1
#endif

#ifndef EF_HAL_GPIO_TOTAL_WAIT_FOR_INT
#define EF_HAL_GPIO_TOTAL_WAIT_FOR_INT 1
#endif

/******************************* UART ****************************************/

typedef void (*efHal_uart_confCB_t)(efHal_dh_t dh, efHal_uart_conf_t const *cfg);
typedef bool (*efHal_uart_sendCB_t)(efHal_dh_t dh, void *pBuf, int32_t size, TickType_t blockTime);
typedef bool (*efHal_uart_recvCB_t)(efHal_dh_t dh, void *pBuf, int32_t size, TickType_t blockTime);

typedef struct
{
    efHal_uart_confCB_t conf;
    efHal_uart_sendCB_t send;
    efHal_uart_recvCB_t recv;
}efHal_uart_callBacks_t;


/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

extern efHal_dh_t efHal_internal_searchFreeSlot(efHal_internal_dhD_t *p_dhD, size_t size, size_t length);

/******************************* GPIO ****************************************/

extern void efHal_internal_gpio_setCallBacks(efHal_gpio_callBacks_t cb);
extern void efHal_internal_gpio_InterruptRoutine(efHal_gpio_id_t id);

/******************************* I2C *****************************************/

extern void efHal_internal_i2c_endOfTransfer(efHal_internal_dhD_t *p_dhD, efHal_i2c_ec_t ec);
extern efHal_dh_t efHal_internal_i2c_deviceReg(efHal_i2c_deviceTransfer_t cb_devTra, void* param);

/******************************* UART ****************************************/
extern efHal_dh_t efHal_internal_uart_deviceReg(efHal_uart_callBacks_t cb, void* param);


/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* EF_HAL_INTERNAL_H_ */
