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
#ifndef EF_HAL_CONFIG_H_
#define EF_HAL_CONFIG_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

#define EF_HAL_I2C_TOTAL_DEVICES        0

#define EF_HAL_SPI_TOTAL_DEVICES        0

#define EF_HAL_GPIO_TOTAL_CALL_BACK     5

#define EF_HAL_GPIO_TOTAL_WAIT_FOR_INT  5

#define EF_HAL_UART_TOTAL_DEVICES   0

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* EF_HAL_CONFIG_H_ */