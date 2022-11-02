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
#
# library
LIBS 				  += extern_lvgl
# version
extern_lvgl_VERSION      = 0.0.0
# library path
extern_lvgl_PATH 			= $(ROOT_DIR)$(DS)externals$(DS)lvgl$(DS)lvgl
# library source path
extern_lvgl_SRC_PATH 	= $(extern_lvgl_PATH)$(DS)src \
                  $(extern_lvgl_PATH)$(DS)src/core \
                  $(extern_lvgl_PATH)$(DS)src/draw \
                  $(extern_lvgl_PATH)$(DS)src/draw/sw \
                  $(extern_lvgl_PATH)$(DS)src/extra \
                  $(extern_lvgl_PATH)$(DS)src/font \
                  $(extern_lvgl_PATH)$(DS)src/gpu \
                  $(extern_lvgl_PATH)$(DS)src/hal \
                  $(extern_lvgl_PATH)$(DS)src/misc \
                  $(extern_lvgl_PATH)$(DS)src/widgets \
                  $(extern_lvgl_PATH)$(DS)src/extra/layouts \
                  $(extern_lvgl_PATH)$(DS)src/extra/layouts/flex \
                  $(extern_lvgl_PATH)$(DS)src/extra/layouts/grid \
                  $(extern_lvgl_PATH)$(DS)src/extra/themes \
                  $(extern_lvgl_PATH)$(DS)src/extra/themes/basic \
                  $(extern_lvgl_PATH)$(DS)src/extra/themes/default \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/calendar \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/chart \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/colorwheel \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/imgbtn \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/keyboard \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/led \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/list \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/menu \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/meter \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/msgbox \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/spinbox \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/spinner \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/tabview \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/tileview \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/win


# library include path
extern_lvgl_INC_PATH 	= $(extern_lvgl_PATH)$(DS)src \
                  $(extern_lvgl_PATH)$(DS)src/core \
                  $(extern_lvgl_PATH)$(DS)src/draw \
                  $(extern_lvgl_PATH)$(DS)src/draw/sw \
                  $(extern_lvgl_PATH)$(DS)src/extra \
                  $(extern_lvgl_PATH)$(DS)src/font \
                  $(extern_lvgl_PATH)$(DS)src/gpu \
                  $(extern_lvgl_PATH)$(DS)src/hal \
                  $(extern_lvgl_PATH)$(DS)src/misc \
                  $(extern_lvgl_PATH)$(DS)src/widgets \
                  $(extern_lvgl_PATH)$(DS)src/extra/layouts \
                  $(extern_lvgl_PATH)$(DS)src/extra/layouts/flex \
                  $(extern_lvgl_PATH)$(DS)src/extra/layouts/grid \
                  $(extern_lvgl_PATH)$(DS)src/extra/themes \
                  $(extern_lvgl_PATH)$(DS)src/extra/themes/basic \
                  $(extern_lvgl_PATH)$(DS)src/extra/themes/default \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/calendar \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/chart \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/colorwheel \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/imgbtn \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/keyboard \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/led \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/list \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/menu \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/meter \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/msgbox \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/spinbox \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/spinner \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/tabview \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/tileview \
                  $(extern_lvgl_PATH)$(DS)src/extra/widgets/win

# library source files
extern_lvgl_SRC_FILES 	= $(foreach extern_lvgl_SRC, $(extern_lvgl_SRC_PATH), $(wildcard $(extern_lvgl_SRC)$(DS)*.c))
