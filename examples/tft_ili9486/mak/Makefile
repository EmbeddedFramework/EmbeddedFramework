###############################################################################
#
# Copyright 2021, Gustavo Muro
# Copyright 2014, 2015, Mariano Cerdeiro
# Copyright 2014, 2015, 2016, Juan Cecconi (Numetron, UTN-FRBA)
# Copyright 2014, 2015, Esteban Volentini (LabMicro, UNT)
# Copyright 2017, Gustavo Muro (DIGI CHECK)
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
						

# Modules needed for this project
MODS += externals$(DS)drivers
MODS += externals$(DS)board
MODS += externals$(DS)freertos
MODS += externals$(DS)cmsis

MODS += modules$(DS)efHal
MODS += modules$(DS)efErrorHdl
MODS += modules$(DS)tft$(DS)ili9486$(DS)3p5_SCREEN_SHIELD
MODS += modules$(DS)bsp_$(BOARD)

