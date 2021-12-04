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
LIBS 				  += ext_lwip
# version
ext_lwip_VERSION      = 0.0.0
# library path
ext_lwip_PATH 			= $(ROOT_DIR)$(DS)externals$(DS)lwip
# library source path
ext_lwip_SRC_PATH 	= 	$(ext_lwip_PATH)$(DS)src$(DS)api \
					$(ext_lwip_PATH)$(DS)src$(DS)apps$(DS)mqtt \
					$(ext_lwip_PATH)$(DS)src$(DS)core \
					$(ext_lwip_PATH)$(DS)src$(DS)core$(DS)ipv4 \
					$(ext_lwip_PATH)$(DS)src$(DS)core$(DS)ipv6 \
					$(ext_lwip_PATH)$(DS)src$(DS)netif \
					$(ext_lwip_PATH)$(DS)src$(DS)netif$(DS)ppp \
					$(ext_lwip_PATH)$(DS)system$(DS)arch \
					$(ext_lwip_PATH)$(DS)system$(DS)OS
						

ext_lwip_SRC_FILES 	= $(foreach ext_lwip_SRC, $(ext_lwip_SRC_PATH), $(wildcard $(ext_lwip_SRC)$(DS)*.c)) 
ext_lwip_SRC_FILES += $(foreach ext_lwip_SRC, $(ext_lwip_SRC_PATH), $(wildcard $(ext_lwip_SRC)$(DS)*.s))

# library include path
ext_lwip_INC_PATH 	= 	$(ext_lwip_PATH)$(DS)src$(DS)include$(DS)compat$(DS)posix$(DS)arpa \
					$(ext_lwip_PATH)$(DS)src$(DS)include$(DS)compat$(DS)posix$(DS)net \
					$(ext_lwip_PATH)$(DS)src$(DS)include$(DS)compat$(DS)posix$(DS)sys \
					$(ext_lwip_PATH)$(DS)src$(DS)include$(DS)lwip \
					$(ext_lwip_PATH)$(DS)src$(DS)include$(DS)lwip$(DS)apps \
					$(ext_lwip_PATH)$(DS)src$(DS)include$(DS)lwip$(DS)priv \
					$(ext_lwip_PATH)$(DS)src$(DS)include$(DS)netif \
					$(ext_lwip_PATH)$(DS)src$(DS)include$(DS)netif$(DS)ppp \
					$(ext_lwip_PATH)$(DS)src$(DS)include \
					$(ext_lwip_PATH)$(DS)system$(DS)arch \
					$(ext_lwip_PATH)$(DS)system
					
                 

