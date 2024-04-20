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
#


ifeq ($(CPU),MKL46Z256VLL4)
LINKER_SCRIPT ?= linkerScript.ld
CFLAGS += -std=gnu99 -mcpu=cortex-m0plus -mthumb -fstack-usage
CFLAGS += -Og -fno-common -g -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants
CFLAGS += -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFRDM_KL46Z -DFREEDOM -DFSL_RTOS_FREE_RTOS -DSDK_DEBUGCONSOLE=0
CFLAGS += -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DARM_MATH_CM0PLUS
AFLAGS += -c -x assembler-with-cpp -g3 -mcpu=cortex-m0plus -mthumb -DARM_MATH_CM0PLUS
endif

LFLAGS += -L "$(ext_base_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)linker$(DS)"
LFLAGS += -T "$(LINKER_SCRIPT)"
LFLAGS += -nostdlib -mcpu=cortex-m0plus -mthumb -Wl,-Map="$(TARGET_NAME).map" -Wl,-gc-sections -Xlinker -print-memory-usage


ext_base_INC_PATH  += $(ext_base_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)inc

ext_base_SRC_FILES += $(wildcard $(ext_base_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)src$(DS)*.c) \
					  $(wildcard $(ext_base_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)src$(DS)*.cpp) \
					  $(wildcard $(ext_base_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)src$(DS)*.s) \
					  $(wildcard $(ext_base_PATH)$(DS)$(ARCH)$(DS)$(CPUTYPE)$(DS)$(CPU)$(DS)src$(DS)*.S)
