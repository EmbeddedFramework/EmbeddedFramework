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
LIBS 				  += mod_bsp_nucleoF767ZI
# version
mod_bsp_nucleoF767ZI_VERSION    = 0.0.0
# library path
mod_bsp_nucleoF767ZI_PATH 		= $(ROOT_DIR)$(DS)modules$(DS)bsp_nucleoF767ZI

# library source path
mod_bsp_nucleoF767ZI_SRC_PATH 	= $(mod_bsp_nucleoF767ZI_PATH)$(DS)src \
							$(mod_bsp_nucleoF767ZI_PATH)$(DS)internals$(DS)src \
							$(mod_bsp_nucleoF767ZI_PATH)$(DS)internals$(DS)Core$(DS)Src \
							$(mod_bsp_nucleoF767ZI_PATH)$(DS)internals$(DS)Core$(DS)Startup


# library include path
mod_bsp_nucleoF767ZI_INC_PATH 	= $(mod_bsp_nucleoF767ZI_PATH)$(DS)inc \
							$(mod_bsp_nucleoF767ZI_PATH)$(DS)internals$(DS)inc \
							$(mod_bsp_nucleoF767ZI_PATH)$(DS)internals$(DS)Core$(DS)Inc


# library source files
mod_bsp_nucleoF767ZI_SRC_FILES 	= $(foreach mod_bsp_nucleoF767ZI_SRC, $(mod_bsp_nucleoF767ZI_SRC_PATH), $(wildcard $(mod_bsp_nucleoF767ZI_SRC)$(DS)*.c))
mod_bsp_nucleoF767ZI_SRC_FILES  += $(foreach mod_bsp_nucleoF767ZI_SRC, $(mod_bsp_nucleoF767ZI_SRC_PATH), $(wildcard $(mod_bsp_nucleoF767ZI_SRC)$(DS)*.s))
