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

# library
LIBS 				  += ext_freertos
# version
ext_freertos_VERSION      = 10.3.1
# library path
ext_freertos_PATH 			= $(ROOT_DIR)$(DS)externals$(DS)freertos
# library source path
ext_freertos_SRC_PATH 	= $(ext_freertos_PATH)$(DS)src								\
				          $(ext_freertos_PATH)$(DS)portable$(DS)$(CPU)

ext_freertos_SRC_FILES 	= $(foreach ext_freertos_SRC, $(ext_freertos_SRC_PATH), $(wildcard $(ext_freertos_SRC)$(DS)*.c)) 
ext_freertos_SRC_FILES += $(foreach ext_freertos_SRC, $(ext_freertos_SRC_PATH), $(wildcard $(ext_freertos_SRC)$(DS)*.cpp)) 
ext_freertos_SRC_FILES += $(foreach ext_freertos_SRC, $(ext_freertos_SRC_PATH), $(wildcard $(ext_freertos_SRC)$(DS)*.s))
ext_freertos_SRC_FILES += $(foreach ext_freertos_SRC, $(ext_freertos_SRC_PATH), $(wildcard $(ext_freertos_SRC)$(DS)*.S))

# library include path
ext_freertos_INC_PATH 	= $(ext_freertos_PATH)$(DS)inc                             \
                          $(ext_freertos_PATH)$(DS)portable$(DS)$(CPU)$(DS)

ifeq ($(SKIP_FREERTOS_HEAP), 1)
	ext_freertos_SRC_FILES := $(filter-out $(ext_freertos_PATH)$(DS)portable$(DS)$(CPU)$(DS)heap_%, $(ext_freertos_SRC_FILES))
endif

