# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
# */

F_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/AI_Engine_Development/*}')

ROOTFS ?= ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2024.2/rootfs.ext4
IMAGE ?= ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2024.2/Image
SDKTARGETSYSROOT ?= ${SYSROOT}

# Makefile input options
TARGET := hw_emu
PFM := tutorial

# File names and locations
GRAPH := aie/graph.cpp
GRAPH_O := libadf.a

KERNEL := s2mm.cpp mm2s.cpp
KERNEL_XO := pl_kernels/s2mm.xo pl_kernels/mm2s.xo

CONFIG_FILE := system.cfg
EMCONFIG_FILE = emconfig.json

EXECUTABLE = host.exe

PACKAGE_OUT = ./package.$(TARGET)

BASE_PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm

# Command-line options
VPP := v++
AIECC := v++ -c --mode aie
AIESIM := aiesimulator
X86SIM := x86simulator
SW_EMU_CMD := ./host_ps_on_x86 a.xclbin
HW_EMU_CMD := ./launch_hw_emu.sh -aie-sim-options ../aiesimulator_output/aiesim_options.txt -add-env AIE_COMPILER_WORKDIR=../Work 

AIE_INCLUDE_FLAGS := --include "$(XILINX_VITIS)/aietools/include" --include "./aie" --include "./data" --include "./aie/kernels" --include "./"
AIE_FLAGS := $(AIE_INCLUDE_FLAGS) --platform $(BASE_PLATFORM) --work_dir ./Work

AIE_FLAGS += --target hw

VPP_XO_FLAGS := -c --mode hls --platform $(BASE_PLATFORM)

VPP_LINK_FLAGS := -l -t $(TARGET) --platform $(BASE_PLATFORM) $(KERNEL_XO) $(GRAPH_O)  --save-temps -g --config $(CONFIG_FILE) -o $(PFM).xsa
VPP_FLAGS := $(VPP_LINK_FLAGS)

GCC_FLAGS := -Wall -c -g \
	     -std=c++17


GCC_INCLUDES += -I$(SDKTARGETSYSROOT)/usr/include/xrt -I$(SDKTARGETSYSROOT)/usr/include

GCC_LIB := -lxrt_coreutil
GCC_LIB += --sysroot=${SDKTARGETSYSROOT}	

LDCLFLAGS := $(GCC_LIB)

.ONESHELL:
.PHONY: clean all kernels aie sim xsa host package run_emu

###
# Guarding Checks. Do not modify.
###
check_defined = \
	$(strip $(foreach 1,$1, \
		$(call __check_defined,$1,$(strip $(value 2)))))

__check_defined = \
	$(if $(value $1),, \
		$(error Undefined $1$(if $2, ($2))))

guard-PLATFORM_REPO_PATHS:
	$(call check_defined, PLATFORM_REPO_PATHS, Set your where you downloaded xilinx_vck190_base_202420_1)

guard-ROOTFS:
	$(call check_defined, ROOTFS, Set to: xilinx-versal-common-v2024.2/rootfs.ext4)

guard-IMAGE:
	$(call check_defined, IMAGE, Set to: xilinx-versal-common-v2024.2/Image)

guard-CXX:
	$(call check_defined, CXX, Run: xilinx-versal-common-v2024.2/environment-setup-aarch64-xilinx-linux)

guard-SDKTARGETSYSROOT:
	$(call check_defined, SDKTARGETSYSROOT, Run: xilinx-versal-common-v2024.2/environment-setup-aarch64-xilinx-linux)

###

all: kernels aie sim xsa host package
sd_card: all

######################################################
# This step compiles the HLS C kernels and creates the *.xo's 
# which is used as the output and from the *.cpp files.
# Note : hw_emu and hw targets use the Unified CLI command to 
# compile HLS kernels


kernels: guard-PLATFORM_REPO_PATHS 
	$(VPP) $(VPP_XO_FLAGS) --config pl_kernels/s2mm.cfg
	$(VPP) $(VPP_XO_FLAGS) --config pl_kernels/mm2s.cfg

aie: $(GRAPH_O)

#AIE or X86 Simulation
sim: $(GRAPH_O)
	$(AIESIM)  --dump-vcd=tutorial --pkg-dir=./Work

#AIE or X86 compilation
$(GRAPH_O): $(GRAPH)
	$(AIECC) $(AIE_FLAGS) $(GRAPH)
	
#####################################################

########################################################
# Once the kernels and graph are generated, you can build
# the hardware part of the design. This creates an xsa
# that will be used to run the design on the platform.
xsa: guard-PLATFORM_REPO_PATHS $(GRAPH_O) $(KERNEL_XO)
	$(VPP) $(VPP_LINK_FLAGS) || (echo "task: [xsa] failed error code: $$?"; exit 1)
	@echo "COMPLETE: .xsa created."
########################################################

############################################################################################################################
# For sw emulation, hw emulation and hardware, compile the PS code and generate the host.exe. This is needed for creating the sd_card.
host: guard-CXX guard-SDKTARGETSYSROOT 
	cd ./sw 
	$(CXX) $(GCC_FLAGS) $(GCC_INCLUDES) -o host.o host.cpp
	$(CXX) *.o $(GCC_LIB) -std=c++17 -o ${EXECUTABLE}
	@echo "COMPLETE: Host application created."
############################################################################################################################

##################################################################################################
package: guard-PLATFORM_REPO_PATHS guard-IMAGE guard-ROOTFS
	cd ./sw
	v++ -p -t ${TARGET} \
		-f ${BASE_PLATFORM} \
		--package.rootfs=${ROOTFS} \
		--package.image_format=ext4 \
		--package.boot_mode=sd \
		--package.kernel_image=${IMAGE} \
		--package.defer_aie_run \
		--package.sd_file embedded_exec.sh \
		--package.sd_file host.exe ../tutorial.xsa ../libadf.a
	@echo "COMPLETE: emulation package created."


###################################################################################################

#Build the design and then run sw/hw emulation 
run: all run_emu

###########################################################################
run_emu: 

# If the target is for HW_EMU, launch the emulator
ifeq (${TARGET},hw_emu)
	cd ./sw
	$(HW_EMU_CMD)
else
	@echo "Hardware build, no emulation executed."
endif

###########################################################################

clean:
	rm -rf _x v++* $(KERNEL_XO) $(GRAPH_O) *.o *.compile_summary* *.xpe xnwOut *.xclbin* *.log *.xsa Work *.db *.csv *$(PFM)* *.jou .Xil
	rm -rf sw/*.log sw/*.xclbin sw/cfg/ sw/launch_hw_emu.sh sw/qemu_dts_files sw/emu_qemu_scripts sw/*.exe sw/_x/ sw/*summary sw/*.o sw/*.elf sw/*.xpe sw/xnwOut sw/Work sw/*.csv sw/*.db sw/*.bin sw/*.BIN sw/*.bif sw/launch_hw_emulator.sh sw/*.txt sw/emulation sw/.Xil ./x86simulator_output
	rm -rf sw/sd_card sw/sd_card.img sw/*.o ./*.exe sw/qeumu* x86simulator_output/ aiesimulator_output/ s2mm/ mm2s/ hls/
