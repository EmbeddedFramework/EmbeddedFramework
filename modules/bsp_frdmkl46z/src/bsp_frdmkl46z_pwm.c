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
#include "bsp_frdmkl46z_pwm.h"
#include "bsp_frdmkl46z_gpio.h"
#include "efHal_internal.h"

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "fsl_tpm.h"
#include "pin_mux.h"

/*==================[macros and typedef]=====================================*/

typedef struct
{
	TPM_Type *tpm;
	tpm_chnl_t chnl;
	efHal_gpio_id_t gpio;
}pwmStruct_t;

/*==================[internal functions declaration]=========================*/

static float _dutyPercentage[EF_HAL_PWM_TOTAL] = {0};

/*==================[internal data definition]===============================*/

static const pwmStruct_t pwmStruct[] =
{
    {TPM1, 0, EF_HAL_D3},     			  /* EF_HAL_PWM0 */
	{TPM0, 2, EF_HAL_D5},     			  /* EF_HAL_PWM1 */
	{TPM0, 4, EF_HAL_D6},     			  /* EF_HAL_PWM2 */
	{TPM0, 1, EF_HAL_D9},     			  /* EF_HAL_PWM3 */
    {TPM0, 5, EF_HAL_GPIO_LED_GREEN},     /* EF_HAL_GPIO_LED_GREEN */
    {TPM0, 2, EF_HAL_GPIO_LED_RED}        /* EF_HAL_GPIO_LED_RED */
};

static SemaphoreHandle_t mutex;
static tpm_chnl_pwm_signal_param_t tpm_chnl_pwm_signal_param[6];

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/
bool setDuty (efHal_pwm_id_t id, uint32_t dutyCount){
	_dutyPercentage[id] = ((float)dutyCount/(float)pwmStruct[id].tpm->MOD*(float)100 + 1);
	uint16_t  mod = pwmStruct[id].tpm->MOD;

	xSemaphoreTake(mutex, portMAX_DELAY);

	bsp_frdmkl46z_internal_gpio_confAsPWM(pwmStruct[id].gpio);

	if(dutyCount >= 0){
		if (dutyCount >= mod) pwmStruct[id].tpm->CONTROLS[pwmStruct[id].chnl].CnV = mod + 1;
		else if(id == EF_HAL_PWM_LED_RED || id == EF_HAL_PWM_LED_GREEN)
			pwmStruct[id].tpm->CONTROLS[pwmStruct[id].chnl].CnV = mod - dutyCount;
		else
			pwmStruct[id].tpm->CONTROLS[pwmStruct[id].chnl].CnV = dutyCount;
	}

	xSemaphoreGive(mutex);

	return 1;
}

bool setPeriod (efHal_pwm_id_t id, uint32_t period_nS){
	uint16_t dutyCount = 0;

	xSemaphoreTake(mutex, portMAX_DELAY);

	TPM_StopTimer(pwmStruct[id].tpm);

	pwmStruct[id].tpm->MOD = (uint32_t)(((float)CLOCK_GetFreq(kCLOCK_PllFllSelClk)/(float)(1U << (pwmStruct[id].tpm->SC & TPM_SC_PS_MASK)) * (float)period_nS * (float)1e-9) - 1);

	dutyCount = _dutyPercentage[id] * pwmStruct[id].tpm->MOD;

	if(dutyCount >= 0){
		if (dutyCount >= pwmStruct[id].tpm->MOD) pwmStruct[id].tpm->CONTROLS[pwmStruct[id].chnl].CnV = pwmStruct[id].tpm->MOD + 1;
		else if(id == EF_HAL_PWM_LED_RED || id == EF_HAL_PWM_LED_GREEN)
			pwmStruct[id].tpm->CONTROLS[pwmStruct[id].chnl].CnV = pwmStruct[id].tpm->MOD - dutyCount;
		else
			pwmStruct[id].tpm->CONTROLS[pwmStruct[id].chnl].CnV = dutyCount;
	}

	TPM_StartTimer(pwmStruct[id].tpm, kTPM_SystemClock);

	xSemaphoreGive(mutex);

	return 1;
}

uint32_t getPeriodCount (efHal_pwm_id_t id){
	return pwmStruct[id].tpm->MOD;
}

uint32_t getPeriodNs (efHal_pwm_id_t id){
	return (uint32_t)(((float)pwmStruct[id].tpm->MOD+1)*(float)1e9/((float)CLOCK_GetFreq(kCLOCK_PllFllSelClk)/(float)(1U << (pwmStruct[id].tpm->SC & TPM_SC_PS_MASK))));
}

void confIntCount (efHal_pwm_id_t id, uint32_t count){

}


/*==================[external functions definition]==========================*/
extern void bsp_frdmkl46z_pwm_init(void)
{
	efHal_pwm_callBacks_t cb;
    tpm_config_t tpm0_config, tpm1_config;

    mutex = xSemaphoreCreateMutex();

    //kCLOCK_PllFllSelClk for TPM
    CLOCK_SetTpmClock(1U);

    /*
	* tpm_config.prescale = kTPM_Prescale_Divide_1;
	* tpm_config.useGlobalTimeBase = false;
	* tpm_config.enableDoze = false;
	* tpm_config.enableDebugMode = false;
	* tpm_config.enableReloadOnTrigger = false;
	* tpm_config.enableStopOnOverflow = false;
	* tpm_config.enableStartOnTrigger = false;
	* tpm_config.enablePauseOnTrigger = false;
	* tpm_config.triggerSelect = kTPM_Trigger_Select_0;
	* tpm_config.triggerSource = kTPM_TriggerSource_External;
	*/
    TPM_GetDefaultConfig(&tpm0_config);
    TPM_GetDefaultConfig(&tpm1_config);

    tpm1_config.prescale = kTPM_Prescale_Divide_64;

    TPM_Init(TPM0, &tpm0_config);
    TPM_Init(TPM1, &tpm1_config);

    for(int i=0; i<6; i++){
        tpm_chnl_pwm_signal_param[i].chnlNumber = i;
        tpm_chnl_pwm_signal_param[i].level = kTPM_HighTrue;
        tpm_chnl_pwm_signal_param[i].dutyCyclePercent = 0;
    }

    TPM_SetupPwm(TPM0, tpm_chnl_pwm_signal_param, 6, kTPM_EdgeAlignedPwm, 1000, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
    TPM_SetupPwm(TPM1, tpm_chnl_pwm_signal_param, 1, kTPM_EdgeAlignedPwm, 1000, CLOCK_GetFreq(kCLOCK_PllFllSelClk));

    TPM_StartTimer(TPM0, kTPM_SystemClock);
    TPM_StartTimer(TPM1, kTPM_SystemClock);

    cb.setDuty = setDuty;
    cb.setPeriod = setPeriod;
    cb.getPeriodCount = getPeriodCount;
    cb.getPeriodNs = getPeriodNs;
    cb.confIntCount = confIntCount;

    efHal_internal_pwm_setCallBacks(cb);
}

/*==================[end of file]============================================*/
