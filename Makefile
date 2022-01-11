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
#
###############################################################################
######################## DO NOT CHANGE THIS FILE ##############################
###############################################################################
# You dont have to change this file, never.
#
# Copy Makefile.example to Makefile.mine and set the variables.
#
# Makefile.mine will be included by this root Makefile and your configurations
# in Makefile.mine will be ignored due to .gitignore.
#
# Please take into account to check and compare your Makefile.mine every time
# that Makefile.config is updated, you may have to adapt your Makefile.mine
# if Makefile.config is changed.
#
###############################################################################
-include Makefile.mine

# Default values for frdmkl46z
ifeq ($(BOARD),frdmkl46z)
ARCH           ?= cortexM0plus
CPUTYPE        ?= KL46
CPU            ?= MKL46Z256VLL4
COMPILER       ?= gcc
endif


# Default values in other case
ARCH           ?= x86
CPUTYPE        ?= ia64
CPU            ?= pcCPU
COMPILER       ?= gcc
BOARD          ?= pcSim

# export defined vars to the environment
export ARCH
export CPUTYPE
export CPU
export COMPILER
export BOARD

DS             ?= /

# dependencies options
#
# Compile and make dependencies, or compile only and skip dependencies
MAKE_DEPENDENCIES ?= 1

###############################################################################
# EmbeddedFirmware version information
EMBEDDED_FRAMEWORK_VER     = 0.0.0

###############################################################################
# get OS
#
# This part of the makefile is used to detect your OS. Depending on the OS
ifeq ($(OS),Windows_NT)
# WINDOWS
# Command line separator
CS                = ;
# Command for multiline echo
MULTILINE_ECHO    = echo -e
# convert paths from cygwin to win (used to convert path for compiler)
define cyg2win
`cygpath -w $(1)`
endef
define cp4c
$(if $(findstring tst_,$(MAKECMDGOALS)),$(1),$(call cyg2win,$(1)))
endef
# Libraries group linker parameters
START_GROUP       = -Xlinker --start-group
END_GROUP         = -Xlinker --end-group
else
# NON WINDOWS OS
# Command line separator
CS                = ;
# Comand for multiline echo
MULTILINE_ECHO    = echo -n
define cyg2win
$(1)
endef
define cp4c
$(1)
endef
UNAME_S           := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
# LINUX
# Libraries group linker parameters
START_GROUP       = -Xlinker --start-group
END_GROUP         = -Xlinker --end-group
endif
ifeq ($(UNAME_S),Darwin)
# MACOS
# Determine wich FTDI Drivers are loaded
ifeq ($(FTDI_KEXT),)
FTDI_KEXT := $(shell kextstat | grep -o com.apple.driver.AppleUSBFTDI )
FTDI_KEXT += $(shell kextstat | grep -o com.FTDI.driver.FTDIUSBSerialDriver )
endif
# Compile in 32 bits mode
ifeq ($(ARCH),x86)
CFLAGS            += -m32 -Wno-typedef-redefinition
# 32 bits non relocable excutable image
LFLAGS            += -m32 -Xlinker -no_pie
# Libraries group linker parameters
START_GROUP       = -all_load
END_GROUP         =
else
START_GROUP       = -Xlinker --start-group
END_GROUP         = -Xlinker --end-group
endif
endif
endif

###############################################################################
# get root dir
ROOT_DIR := .
# out dir
OUT_DIR   = $(ROOT_DIR)$(DS)out
# object dir
OBJ_DIR  = $(OUT_DIR)$(DS)obj
# lib dir
LIB_DIR  = $(OUT_DIR)$(DS)lib
# bin dir
BIN_DIR  = $(OUT_DIR)$(DS)bin
# etc dir (configuration dir)
ETC_DIR = $(OUT_DIR)$(DS)etc

# include needed project
ifneq ($(PROJECT_PATH),)
include $(PROJECT_PATH)$(DS)mak$(DS)Makefile
endif
# base module is always needed and included
MODS += modules$(DS)base
# include needed modules
include $(foreach module, $(MODS), $(module)$(DS)mak$(DS)Makefile)

.DEFAULT_GOAL := $(PROJECT_NAME)

# add include files
INCLUDE += $(foreach LIB, $(LIBS), $($(LIB)_INC_PATH))
#Adds include project file if We wanto to do a project build
ifneq ($(findstring tst_, $(MAKECMDGOALS)),tst_)
CFLAGS  += $(foreach inc, $(INC_FILES), -I$(call cp4c,$(inc)))
endif
CFLAGS  += $(foreach inc, $(INCLUDE), -I$(inc))
CFLAGS  += -DARCH=$(ARCH) -DCPUTYPE=$(CPUTYPE) -DCPU=$(CPU) -DBOARD=$(BOARD)
TARGET_NAME ?= $(BIN_DIR)$(DS)$(PROJECT_NAME)
LD_TARGET = $(TARGET_NAME).$(LD_EXTENSION)
# create list of object files for a Lib (without DIR), based on source file %.c and %.s
$(foreach LIB, $(LIBS), $(eval $(LIB)_OBJ_FILES =  $(notdir $(patsubst %.c,%.o,$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(patsubst %.cpp,%.o,$($(LIB)_SRC_FILES))))))))
# Complete list of object files (without DIR), based on source file %.c and %.s
$(foreach LIB, $(LIBS), $(eval LIBS_OBJ_FILES += $($(LIB)_OBJ_FILES)))
# Complete Libs Source Files for debug Info
$(foreach LIB, $(LIBS), $(eval LIBS_SRC_FILES += $($(LIB)_SRC_FILES)))
# Complete Libs Source Dirs for vpath search (duplicates removed by sort)
$(foreach LIB, $(LIBS), $(eval LIBS_SRC_DIRS += $(sort $(dir $($(LIB)_SRC_FILES)))))
# Add the search patterns
vpath %.c $($(PROJECT_NAME)_SRC_PATH)
vpath %.c $(LIBS_SRC_DIRS)
vpath %.s $(LIBS_SRC_DIRS)
vpath %.S $(LIBS_SRC_DIRS)
vpath %.cpp $(LIBS_SRC_DIRS)
vpath %.o $(OBJ_DIR)

#rule for library
define librule
$(LIB_DIR)$(DS)$(strip $(1)).a : $(2)
	@echo ' '
	@echo ===============================================================================
	@echo Creating library $(1)
	$(AR) -crs $(LIB_DIR)$(DS)$(strip $(1)).a $(foreach obj,$(2),$(OBJ_DIR)$(DS)$(obj))
	@echo ' '
endef

OBJ_FILES = $(notdir $(patsubst %.c,%.o,$(patsubst %.s,%.o,$(patsubst %.S,%.o,$(SRC_FILES)))))

# create rule for library
# lib.a : lib_OBJ_FILES.o
$(foreach LIB, $(LIBS), $(eval $(call librule, $(LIB), $($(LIB)_OBJ_FILES))) )

$(foreach LIB, $(LIBS), $(eval $(LIB) : $(LIB_DIR)$(DS)$(LIB).a ) )

###################### START UNIT TEST PART OF MAKE FILE ######################
# Gets all Modules Names
DIRS := $(sort $(dir $(wildcard modules$(DS)*$(DS))))
ALL_MODS := $(subst modules, , $(DIRS))
ALL_MODS := $(subst $(DS), , $(ALL_MODS))

MOCKS_OUT_DIR = $(OUT_DIR)$(DS)ceedling$(DS)mocks
RUNNERS_OUT_DIR = $(OUT_DIR)$(DS)ceedling$(DS)runners

FILES_TO_MOCK = $(foreach DIR, $(DIRS), $(wildcard $(DIR)inc$(DS)*.h))

FILES_MOCKED = $(foreach MOCKED, $(FILES_TO_MOCK), $(MOCKS_OUT_DIR)$(DS)mock_$(notdir $(MOCKED)))


###############################################################################
# rule for tst_<mod>[_file]
ifeq ($(findstring tst_, $(MAKECMDGOALS)),tst_)

# get module to be tested and store it in tst_mod variable
tst_mod = $(firstword $(filter-out tst,$(subst _, ,$(MAKECMDGOALS))))

# get file to be tested (if present) and store it in tst_file
# this shall be done multiple times, one time for each possible _, no 3 _ are supported in the test file name
tst_file := $(word 2,$(filter-out tst,$(subst _, ,$(MAKECMDGOALS))))
ifneq ($(word 3,$(filter-out tst,$(subst _, ,$(MAKECMDGOALS)))),)
tst_file := $(join $(tst_file),_$(word 3,$(filter-out tst,$(subst _, ,$(MAKECMDGOALS)))))
endif
ifneq ($(word 4,$(filter-out tst,$(subst _, ,$(MAKECMDGOALS)))),)
tst_file := $(join $(tst_file),_$(word 4,$(filter-out tst,$(subst _, ,$(MAKECMDGOALS)))))
endif
ifneq ($(word 5,$(filter-out tst,$(subst _, ,$(MAKECMDGOALS)))),)
tst_file := $(join $(tst_file),_$(word 5,$(filter-out tst,$(subst _, ,$(MAKECMDGOALS)))))
endif

# if tst_file is all the variable shall be reset and all tests shall be executed
ifeq ($(tst_file),all)
tst_file :=
endif

# include corresponding makefile
include modules$(DS)$(tst_mod)$(DS)mak$(DS)Makefile
# include test makefile
include modules$(DS)$(tst_mod)$(DS)test$(DS)utest$(DS)mak$(DS)Makefile

# get list of unit test sources
ifneq ($(tst_file),)
# definitions if to run only a specific unit test

# include modules needed for this module
include $(foreach mod,$($(tst_mod)_TST_MOD),$(mod)$(DS)mak$(DS)Makefile)

MTEST_SRC_FILES = $($(tst_mod)_PATH)$(DS)test$(DS)utest$(DS)src$(DS)test_$(tst_file).c

UNITY_INC = externals$(DS)ceedling$(DS)vendor$(DS)unity$(DS)src                     \
            externals$(DS)ceedling$(DS)vendor$(DS)cmock$(DS)src                     \
            out$(DS)ceedling$(DS)mocks                                              \
            $(foreach mod,$($(tst_mod)_TST_MOD),$($(mod)_INC_PATH))                 \
            $($(tst_mod)_TST_INC_PATH)                                              \
			$($(tst_mod)_INC_PATH)

UNITY_SRC = modules$(DS)$(tst_mod)$(DS)test$(DS)utest$(DS)src$(DS)test_$(tst_file).c \
            $(RUNNERS_OUT_DIR)$(DS)test_$(tst_file)_Runner.c                         \
            modules$(DS)$(tst_mod)$(DS)src$(DS)$(tst_file).c                         \
            externals$(DS)ceedling$(DS)vendor$(DS)unity$(DS)src$(DS)unity.c          \
            externals$(DS)ceedling$(DS)vendor$(DS)cmock$(DS)src$(DS)cmock.c          \
            $(foreach file,$(filter-out $(tst_file).c,$(notdir $($(tst_mod)_SRC_FILES))), out$(DS)ceedling$(DS)mocks$(DS)mock_$(file)) \
            $(foreach mods,$($(tst_mod)_TST_MOD), $(foreach files, $(notdir $($(mods)_SRC_FILES)), out$(DS)ceedling$(DS)mocks$(DS)mock_$(files))) \
            $(foreach tst_mocks, $($(tst_mod)_TST_MOCKS), out$(DS)ceedling$(DS)mocks$(DS)mock_$(tst_mocks))
# Needed Unity Obj files
UNITY_OBJ = $(notdir $(UNITY_SRC:.c=.o))
# Add the search patterns
$(foreach U_SRC, $(sort $(dir $(UNITY_SRC))), $(eval vpath %.c $(U_SRC)))

CFLAGS  += -ggdb -c #-Wall -Werror #see issue #28
CFLAGS  += $(foreach inc, $(UNITY_INC), -I$(inc))
CFLAGS  += -DARCH=$(ARCH) -DCPUTYPE=$(CPUTYPE) -DCPU=$(CPU) -DUNITY_EXCLUDE_STDINT_H -DCIAA_UNIT_TEST --coverage

else
# get all test target for the selected module
MTEST := $(notdir $(wildcard $($(tst_mod)_PATH)$(DS)test$(DS)utest$(DS)src$(DS)test_*.c))
MTEST := $(subst test_,,$(MTEST))
MTEST := $(subst .c,,$(MTEST))
MTEST := $(foreach tst, $(MTEST),tst_$(tst_mod)_$(tst))


endif

tst_link: $(UNITY_OBJ)
	@echo ' '
	@echo ===============================================================================
	@echo Linking Test
	gcc $(addprefix $(OBJ_DIR)$(DS),$(UNITY_OBJ)) -lgcov -o out$(DS)bin$(DS)$(tst_file).bin

# rule for tst_<mod>_<file>
tst_$(tst_mod)_$(tst_file): $(RUNNERS_OUT_DIR)$(DS)$(notdir $(MTEST_SRC_FILES:.c=_Runner.c)) tst_link
	@echo ' '
	@echo ===============================================================================
	@echo Testing from module $(tst_mod) the file $(tst_file)
	@echo === CEEDLING START ====
	$(BIN_DIR)$(DS)$(tst_file).bin
	@echo === CEEDLING END ===
	gcov -abclu modules$(DS)$(tst_mod)$(DS)src$(DS)$(tst_file).c -o out$(DS)obj$(DS)

# rule for tst_<mod>
tst_$(tst_mod)_all:
	@echo ' '
	@echo ===============================================================================
	@echo Testing the module $(tst_mod)
	@echo Testing $(MTEST)
	$(foreach tst,$(MTEST),make $(tst) $(CS))

tst_$(tst_mod):
	@echo ' '
	@echo ===============================================================================
	@echo For the module $(tst_mod) following units can be tested:
	@$(MULTILINE_ECHO) " $(foreach unit, $(MTEST),     $(unit)\n)"


#tst_$(tst_mod): $(MTEST_SRC_FILES:.c=_Runner.c) tst_link
#	@echo ===============================================================================
#	@echo Testing the module $(tst_mod)
#	@$(MULTILINE_ECHO) "Testing following .c Files: \n $(foreach src, $($(tst_mod)_SRC_FILES),     $(src)\n)"
#	@$(MULTILINE_ECHO) "Following Unity Test found: \n $(foreach src, $(MTEST_SRC_FILES),     $(src)\n)"
#	out/bin/test.bin

endif

results:
	externals$(DS)lcov$(DS)bin$(DS)lcov -c -d . -o coverage.info -b .
	externals$(DS)lcov$(DS)bin$(DS)genhtml coverage.info --output-directory $(OUT_DIR)$(DS)coverage

###############################################################################
# rule to generate the mocks
mocks:
	@echo ' '
	@echo ===============================================================================
	@$(MULTILINE_ECHO) "Creating Mocks for: \n $(foreach mock, $(FILES_TO_MOCK),     $(mock)\n)"
	ruby externals$(DS)ceedling$(DS)vendor$(DS)cmock$(DS)lib$(DS)cmock.rb -omodules$(DS)tools$(DS)ceedling$(DS)project.yml $(FILES_TO_MOCK)

###############################################################################
# rule to check trailing spaces
code_sanity:
	@echo ' '
	@echo ===============================================================================
	@echo Checking for trailing spaces
	@./modules/tools/scripts/check_trailing_spaces.sh

###############################################################################
# rule to inform about all available tests
tst:
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               Unit Tests                                                    |"
	@echo "+-----------------------------------------------------------------------------+"
	@$(MULTILINE_ECHO) "Following tst rules have been created:\n $(foreach TST,$(ALL_MODS),     tst_$(TST): run unit tests of $(TST)\n)"


$(RUNNERS_OUT_DIR)$(DS)test_%_Runner.c : test_%.c
	@echo ' '
	@echo ===============================================================================
	@echo Creating Runner for $<
	@echo "                 in $(RUNNERS_OUT_DIR)$(DS)$(notdir $@)"
	ruby externals$(DS)ceedling$(DS)vendor$(DS)unity$(DS)auto$(DS)generate_test_runner.rb $< $(RUNNERS_OUT_DIR)$(DS)$(notdir $@) modules$(DS)tools$(DS)ceedling$(DS)project.yml

###################### ENDS UNIT TEST PART OF MAKE FILE #######################

###############################################################################

# Rule to compile
%.o : %.c
	@echo ' '
	@echo ===============================================================================
	@echo Compiling 'c' file: $<
	@echo ' '
	$(CC) $(CFLAGS) $(call cp4c,$<) -o $(OBJ_DIR)$(DS)$@
ifeq ($(MAKE_DEPENDENCIES),1)
	@echo ' '
	@echo Generating dependencies...
	$(CC) -MM $(CFLAGS) $(call cp4c,$<) > $(OBJ_DIR)$(DS)$(@:.o=.d)
else
	@echo ' '
	@echo Skipping make dependencies...
endif

%.o : %.cpp
	@echo ' '
	@echo ===============================================================================
	@echo Compiling 'c++' file: $<
	@echo ' '
	$(CPP) $(CFLAGS) $(call cp4c,$<) -o $(OBJ_DIR)$(DS)$@

%.o : %.s
	@echo ' '
	@echo ===============================================================================
	@echo Compiling 'asm' file: $<
	@echo ' '
	$(AS) $(AFLAGS) $(call cp4c,$<) -o $(OBJ_DIR)$(DS)$@

%.o : %.S
	@echo ' '
	@echo ===============================================================================
	@echo Compiling 'asm' with C preprocessing file: $<
	@echo ' '
	$(CC) $(CFLAGS) -x assembler-with-cpp $(call cp4c,$<) -o $(OBJ_DIR)$(DS)$@


###############################################################################
# Incremental Build (IDE: Build)
# link rule

# New rules for LIBS dependencies
$(foreach LIB, $(LIBS), $(eval -include $(addprefix $(OBJ_DIR)$(DS),$($(LIB)_OBJ_FILES:.o=.d))))
# New rules for project dependencies
$(foreach LIB, $(LIBS), $(eval -include $(addprefix $(OBJ_DIR)$(DS),$(OBJ_FILES:.o=.d))))
# libs with contains sources
LIBS_WITH_SRC	= $(foreach LIB, $(LIBS), $(if $(filter %.c,$($(LIB)_SRC_FILES)),$(LIB)))

$(PROJECT_NAME) : $(rtos_GENERATED_FILES) $(LIBS_WITH_SRC) $(OBJ_FILES)
	@echo ' '
	@echo ===============================================================================
	@echo Linking file: $(LD_TARGET)
	@echo ' '
	$(CC) $(foreach obj,$(OBJ_FILES),$(OBJ_DIR)$(DS)$(obj)) $(START_GROUP) $(foreach lib, $(LIBS_WITH_SRC), $(LIB_DIR)$(DS)$(lib).a) $(foreach lib, $(EXTERN_LIBS), $(lib)) $(END_GROUP) -o $(LD_TARGET) $(LFLAGS)
	@echo ' '
	@echo ===============================================================================
	@echo Post Building $(PROJECT_NAME)
	@echo ' '
	$(POST_BUILD)

###############################################################################
# debug rule
-include modules$(DS)tools$(DS)gdb$(DS)mak$(DS)Makefile
debug: $(PROJECT_NAME)
# if CPU is not entered shows an error
ifeq ($(CPU),)
	@echo ERROR: The CPU variable of your makefile is empty.
else
	@echo ===============================================================================
	@echo Starting GDB...
	@echo ' '
	$(GDB) $(GDB_FLAGS)
endif

###############################################################################
# doxygen
doxygen:
	@echo running doxygen
	doxygen modules$(DS)tools$(DS)doxygen$(DS)doxygen.cnf

###############################################################################
# Take make arguments into MAKE_ARGS variable
MAKE_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
# ...and turn them into do-nothing targets
$(eval $(MAKE_ARGS):;@:)

###############################################################################
# version
ifeq ($(MAKECMDGOALS),version)
include $(foreach module, $(ALL_MODS), modules$(DS)$(module)$(DS)mak$(DS)Makefile)
endif
version:
	@echo EmbeddedFirmware version: $(EMBEDDED_FRAMEWORK_VER)
	@$(MULTILINE_ECHO) " $(foreach mod, $(ALL_MODS), $(mod): $($(mod)_VERSION)\n)"

###############################################################################
# help
help:
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               General Help                                                  |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo info................: general information about the make environment
	@echo info_\<mod\>..........: same as info but reporting information of a library
	@echo info_ext_\<mod\>......: same as info_\<mod\> but for an external library
	@echo version.............: dislpays the version of each module
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               Unit Tests                                                    |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo mocks...............: generate the mocks for all header files
	@echo tst.................: displays possible tests
	@echo tst_\<mod\>...........: shows all unit test of a specific module
	@echo tst_\<mod\>_\<unit\>....: runs the specific unit test
	@echo tst_\<mod\>_all.......: runs all unit tests of a specific module
	@echo results.............: create results report
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               Debugging / Running / Programming                             |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo "run.........................: execute the binary file (Win/Posix only)"
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               Building                                                      |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo clean...............: cleans generated, object, binary, etc. files
	@echo all.................: performs make clean and make

###############################################################################
# info for  aspecific module
ifeq ($(findstring info_, $(MAKECMDGOALS)),info_)
info_mod = $(subst info_,,$(MAKECMDGOALS))
ext = 0

ifeq ($(findstring info_ext_, $(MAKECMDGOALS)),info_ext_)
info_mod = $(subst info_ext_,,$(MAKECMDGOALS))
ext = 1
endif

ifeq ($(ext),1)
# include corresponding makefile
include externals$(DS)$(info_mod)$(DS)mak$(DS)Makefile
else
# include corresponding makefile
include modules$(DS)$(info_mod)$(DS)mak$(DS)Makefile
endif

# create the corresponding info_<mod> rule
info_ext_$(info_mod) :
	@echo ===============================================================================
	@echo Info of ext_$(info_mod)
	@echo Path........: $(ext_$(info_mod)_PATH)
	@$(MULTILINE_ECHO) "Include path: \n $(foreach inc, $(ext_$(info_mod)_INC_PATH),     $(inc)\n)"
	@echo Source path.: $(ext_$(info_mod)_SRC_PATH)
	@$(MULTILINE_ECHO) "Source files:\n $(foreach src, $(ext_$(info_mod)_SRC_FILES),     $(src)\n)"

# create the corresponding info_<mod> rule
info_$(info_mod) :
	@echo ===============================================================================
	@echo Info of $(info_mod)
	@echo Path........: $($(info_mod)_PATH)
	@$(MULTILINE_ECHO) "Include path: \n $(foreach inc, $($(info_mod)_INC_PATH),     $(inc)\n)"
	@echo Source path.: $($(info_mod)_SRC_PATH)
	@echo $(INC_FILES)
	@$(MULTILINE_ECHO) "Source files:\n $(foreach src, $($(info_mod)_SRC_FILES),     $(src)\n)"
endif

###############################################################################
# information
info:
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               Enable Config Info                                            |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo Project Path.......: $(PROJECT_PATH)
	@echo Project Name.......: $(PROJECT_NAME)
	@echo BOARD/ARCH/CPUTYPE/CPU...: $(BOARD)/$(ARCH)/$(CPUTYPE)/$(CPU)
	@echo enable modules.....: $(MODS)
	@echo libraries..........: $(LIBS)
	@echo libraris with srcs.: $(LIBS_WITH_SRC)
	@echo external libraries.: $(EXTERN_LIBS)
#	@echo Lib Src dirs.......: $(LIBS_SRC_DIRS)
#	@echo Lib Src Files......: $(LIBS_SRC_FILES)
#	@echo Lib Obj Files......: $(LIBS_OBJ_FILES)
#	@echo Project Src Path...: $($(PROJECT_NAME)_SRC_PATH)
	@echo Includes...........: $(INCLUDE)
	@echo use make info_\<mod\>: to get information of a specific module. eg: make info_posix
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               FRAMEWORK Firmware Info                                       |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo EmbeddedFirmware ver..: $(EMBEDDED_FRAMEWORK_VER)
	@echo Available modules..: $(ALL_MODS)
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               Compiler Info                                                 |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo Compiler...........: $(COMPILER)
	@echo CC.................: $(CC)
	@echo AR.................: $(AR)
	@echo LD.................: $(LD)
	@echo Compile C Flags....: $(CFLAGS)
	@echo Compile ASM Flags..: $(AFLAGS)
	@echo Target Name........: $(TARGET_NAME)
	@echo Src Files..........: $(SRC_FILES)
	@echo Obj Files..........: $(OBJ_FILES)
	@echo Linker Flags.......: $(LFLAGS)
	@echo Linker Extension...: $(LD_EXTENSION)
	@echo Linker Target......: $(LD_TARGET)
	@echo ext_base_SRC_FILES.: $(ext_base_SRC_FILES)
###############################################################################
# clean
.PHONY: clean all
clean:
	@echo Removing libraries
	@rm -rf $(LIB_DIR)$(DS)*
	@echo Removing bin files
	@rm -rf $(BIN_DIR)$(DS)*
	@echo Removing mocks
	@rm -rf $(MOCKS_OUT_DIR)$(DS)*
	@echo Removing Unity Runners files
	@rm -rf $(RUNNERS_OUT_DIR)$(DS)*
	@echo Removing doxygen files
	@rm -rf $(OUT_DIR)$(DS)doc$(DS)*
	@echo Removing ci outputs
	@rm -rf $(OUT_DIR)$(DS)ci$(DS)*
	@echo Removing coverage
	@rm -rf $(OUT_DIR)$(DS)coverage$(DS)*
	@echo Removing object files
	@rm -rf $(OBJ_DIR)$(DS)*
	@echo Removing Download files
	@rm -rf $(OUT_DIR)$(DS)download

###############################################################################
# Incremental Build (IDE: Build)
all: clean
	make

###############################################################################
# Run the bin file
run:
# if windows or posix shows an error
ifeq ($(ARCH),x86)
	@echo ' '
	@echo ===============================================================================
	@echo Running the file: $(LD_TARGET)
	$(LD_TARGET)
else
	@echo ERROR: You can not run this binary due to you are not in Windows nor Linux ARCH
endif
