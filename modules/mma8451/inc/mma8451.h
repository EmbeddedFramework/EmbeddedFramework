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
#ifndef MMA8451_H_
#define MMA8451_H_

/*==================[inclusions]=============================================*/
#include "efHal.h"

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/
typedef enum
{
    MMA8451_ASLP_50hz = 0b00,
    MMA8451_ASLP_12p5hz = 0b01,
    MMA8451_ASLP_6p25hz = 0b10,
    MMA8451_ASLP_1p56hz = 0b11,
}mma8451_ASLP_t;

typedef enum
{
    MMA8451_DR_800hz = 0b000,
    MMA8451_DR_400hz = 0b001,
    MMA8451_DR_200hz = 0b010,
    MMA8451_DR_100hz = 0b011,
    MMA8451_DR_50hz = 0b100,
    MMA8451_DR_12p5hz = 0b101,
    MMA8451_DR_6p25hz = 0b110,
    MMA8451_DR_1p56hz = 0b111,
}mma8451_DR_t;

typedef struct
{
    unsigned ACTIVE:1;
    unsigned F_READ:1;
    unsigned LNOISE:1;
    unsigned DR:3;
    unsigned ASLP_RATE:2;
}mma8451_ctrlReg1_t;

typedef struct
{
    unsigned INT_EN_DRDY:1;
    unsigned :1;
    unsigned INT_EN_FF_MT:1;
    unsigned INT_EN_PULSE:1;
    unsigned INT_EN_LNDPRT:1;
    unsigned INT_EN_TRANS:1;
    unsigned INT_EN_FIFO:1;
    unsigned INT_EN_ASLP:1;
}mma8451_ctrlReg4_t;                /* Interrupt enable register */

typedef struct
{
    unsigned INT_CFG_DRDY:1;
    unsigned :1;
    unsigned INT_CFG_FF_MT:1;
    unsigned INT_CFG_PULSE:1;
    unsigned INT_CFG_LNDPRT:1;
    unsigned INT_CFG_TRANS:1;
    unsigned INT_CFG_FIFO:1;
    unsigned INT_CFG_ASLP:1;
}mma8451_ctrlReg5_t;                /* Interrupt configuration register */

typedef struct
{
    int16_t accX;
    int16_t accY;
    int16_t accZ;
}mma8451_accIntCount_t;                /* acceleration in internal counts */

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
extern void mma8451_init(efHal_dh_t dh);

extern void mma8451_setDataRate(mma8451_DR_t rate);

extern void mma8451_setCtrlReg1(mma8451_ctrlReg1_t reg1);

extern mma8451_accIntCount_t mma8451_getAccIntCount(void);

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* MMA8451_H_ */
