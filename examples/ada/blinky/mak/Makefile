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
###############################################################################

PROJECT_NAME               = $(lastword $(subst $(DS), , $(PROJECT_PATH)))

# Internal modules
$(PROJECT_NAME)_SRC_PATH  += $(PROJECT_PATH)$(DS)application$(DS)src \
							 $(PROJECT_PATH)$(DS)appBoard$(DS)src

SRC_FILES 			 += $(foreach application_SRC, $($(PROJECT_NAME)_SRC_PATH), $(wildcard $(application_SRC)$(DS)*.c)) 

INC_FILES            += $(PROJECT_PATH)$(DS)application$(DS)inc \
						$(PROJECT_PATH)$(DS)appBoard$(DS)inc
						
EXTERN_LIBS += $(PROJECT_PATH)$(DS)lib$(DS)libl1.a
						

# Modules needed for this project
MODS += externals$(DS)drivers
MODS += externals$(DS)board
MODS += externals$(DS)freertos
MODS += externals$(DS)cmsis

MODS += modules$(DS)efHal
MODS += modules$(DS)efErrorHdl
MODS += modules$(DS)bsp_$(BOARD)


############################################ 
#  Ada
############################################

# to cross compile you must define GPRBUILD, GPRCLEAN and LIBGNAT
# for example: 
# export GPRCLEAN=/path/to/GNAT/2021-arm-elf/bin/gprclean
# export GPRBUILD=/path/to/GNAT/2021-arm-elf/bin/gprbuild
# export LIBGNAT=/path/to/GNAT/2021-arm-elf/arm-eabi/lib/gnat/zfp-cortex-m7f/adalib/libgnat.a
# remark: remember build your library target:
# ...GNAT/2021-arm-elf/arm-eabi/lib/gnat/zfp-cortex-m7f$ $GPRBUILD runtime_build.gpr

ifeq ($(BOARD),nucleoF767ZI)
	export TARGET = arm-eabi
	export RUNTIME = zfp-cortex-m7f
	EXTERN_LIBS += $(LIBGNAT)
endif

ifeq ($(BOARD),pcSim)
	EXTERN_LIBS += -lgnat
endif

GPRBUILD ?= gprbuild
GPRCLEAN ?= gprclean

ada_build:	
			$(GPRBUILD) $(PROJECT_PATH)$(DS)static_lib.gpr

ada_clean:	
			$(GPRCLEAN) $(PROJECT_PATH)$(DS)static_lib.gpr
