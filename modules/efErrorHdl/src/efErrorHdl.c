/*
###############################################################################
#
# Copyright 2022, Gustavo Muro
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
#include "efErrorHdl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"

/*==================[macros and typedef]=====================================*/

#ifndef EF_ERROR_HDL_TOTAL_ERROR_TASK
#define EF_ERROR_HDL_TOTAL_ERROR_TASK   2
#endif

#define INDEX_LOCAL_STORAGE             0

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

static efErrorHdlInfo_t efErrorHdlInfo[EF_ERROR_HDL_TOTAL_ERROR_TASK];


/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static int getSlot(void)
{
    int i;

    for (i = 0 ; i < EF_ERROR_HDL_TOTAL_ERROR_TASK ; i++)
    {
        if (efErrorHdlInfo[i].type == EF_ERROR_HDL_NO_ERROR)
            break;
    }

    if (i >= EF_ERROR_HDL_TOTAL_ERROR_TASK)
        i = -1;

    return i;
}

/*==================[external functions definition]==========================*/

extern void efErrorHdl_init(void)
{
    int i;

    for (i = 0 ; i < EF_ERROR_HDL_TOTAL_ERROR_TASK ; i++)
    {
        efErrorHdlInfo[i].type = EF_ERROR_HDL_NO_ERROR;
    }
}

extern void efErrorHdl_errorFull(efErrorHdlType_t type, char *msg,
        const char func[], int line)
{
    int slot;

    vPortEnterCritical();

    slot = getSlot();

    if (slot < 0)
    {
        type = EF_ERROR_HDL_NO_FREE_SLOT;
        msg = "No free slot for Error";
        slot = 0;
    }
    else
    {
        efErrorHdlInfo[slot].type = type;
        strncpy(efErrorHdlInfo[slot].msg, msg, EF_ERROR_HDL_STR_MSG_LENGTH);
    }

    strncpy(efErrorHdlInfo[slot].func, func, EF_ERROR_HDL_STR_FUNC_LENGTH);
    efErrorHdlInfo[slot].line = line;

    vTaskSetThreadLocalStoragePointer(NULL, INDEX_LOCAL_STORAGE, &efErrorHdlInfo[slot]);

    vPortExitCritical();
}

extern efErrorHdlType_t efErrorHdl_getErrorType(void)
{
    efErrorHdlInfo_t *p_efErrorHdlInfo;
    efErrorHdlType_t ret = EF_ERROR_HDL_NO_ERROR;

    p_efErrorHdlInfo = pvTaskGetThreadLocalStoragePointer(NULL, INDEX_LOCAL_STORAGE);

    if (p_efErrorHdlInfo != NULL)
        ret = p_efErrorHdlInfo->type;

    return ret;
}

extern efErrorHdlInfo_t* efErrorHdl_getErrorInfo(void)
{
    efErrorHdlInfo_t *p_efErrorHdlInfo;
    p_efErrorHdlInfo = pvTaskGetThreadLocalStoragePointer(NULL, INDEX_LOCAL_STORAGE);
    return p_efErrorHdlInfo;
}

extern void efErrirHdl_freeError(void)
{
    efErrorHdlInfo_t *p_efErrorHdlInfo;

    p_efErrorHdlInfo = pvTaskGetThreadLocalStoragePointer(NULL, INDEX_LOCAL_STORAGE);

    if (p_efErrorHdlInfo != NULL)
    {
        vPortEnterCritical();
        p_efErrorHdlInfo->type = EF_ERROR_HDL_NO_ERROR;
        vTaskSetThreadLocalStoragePointer(NULL, INDEX_LOCAL_STORAGE, NULL);
        vPortExitCritical();
    }
}

/*==================[end of file]============================================*/
