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
#ifndef EF_ERROR_HDL_H_
#define EF_ERROR_HDL_H_

/*==================[inclusions]=============================================*/

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros and typedef]=====================================*/

typedef enum
{
    EF_ERROR_HDL_NO_ERROR = 0,
    EF_ERROR_HDL_NULL_POINTER,
    EF_ERROR_HDL_ASSERT,
    EF_ERROR_HDL_NO_FREE_SLOT,
    EF_ERROR_HDL_INVALID_PARAMETER,

    EF_ERROR_HDL_I2C = 100,

    EF_ERROR_HDL_APPLICATION = 1000,

}efErrorHdlType_t;

#define EF_ERROR_HDL_STR_FUNC_LENGTH    16
#define EF_ERROR_HDL_STR_MSG_LENGTH     16

typedef struct
{
    efErrorHdlType_t type;
    char func[EF_ERROR_HDL_STR_FUNC_LENGTH];
    int line;
    char msg[EF_ERROR_HDL_STR_MSG_LENGTH];
}efErrorHdlInfo_t;

/* try catch
 * example:
 * EF_TRY
 * {
 *     sentence1();
 *     EF_CHECK;
 *     sentence1();
 *     EF_CHECK;
 * }
 * EF_CATCH
 * {
 *      efErrorHdlInfo_t* errorInfo;
 *      errorInfo = efErrorHdl_getErrorInfo();
 *      yourCode();
 * }
 * EF_ENDTRY;
 */

#define EF_TRY      \
{                   \
    int ef_hadError = 0;    \
    do{                 \
        if (ef_hadError == 0)   \
        {

#define EF_CATCH  }else

#define EF_ENDTRY       \
        ef_hadError = 0;    \
    }while (ef_hadError);       \
}

#define EF_CHECK if (efErrorHdl_getErrorType() != EF_ERROR_HDL_NO_ERROR) {ef_hadError = 1; continue;}

/*==================[external data declaration]==============================*/

/*==================[external functions declaration]=========================*/
#ifdef EF_ERROR_HDL_ENABLE
extern void efErrorHdl_init(void);

#define efErrorHdl_error(type, msg)     efErrorHdl_errorFull(type, msg, __FUNCTION__, __LINE__)

#define efErrorHdl_assert(expr) \
    ((expr) ? (void)0 : efErrorHdl_errorFull(EF_ERROR_HDL_ASSERT, "", __FUNCTION__, __LINE__)

extern void efErrorHdl_errorFull(efErrorHdlType_t type, char *msg,
        const char func[], int line);

extern efErrorHdlType_t efErrorHdl_getErrorType(void);
extern efErrorHdlInfo_t* efErrorHdl_getErrorInfo(void);

#else
#define efErrorHdl_error(type, msg)
#endif

/*==================[cplusplus]==============================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* EF_ERROR_HDL_H_ */
