#!/bin/bash

#Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

# Make sure the SHELL is in /bin/bash if not do
# echo $SHELL
# export SHELL=/bin/bash
# echo $SHELL

unset LD_LIBRARY_PATH
export PLATFORM_REPO_PATH=<PATH-TO-PLATFORMS-DIRECTORY>
export XILINX_VITIS=<PATH-TO-VITIS-BUILD-DIRECTORY>/Vitis/2024.2
export COMMON_IMAGE_VERSAL=<PATH TO xilinx-versal-common-v2024.2 - DIRECTORY>
export BSP_DIR=<PATH TO xilinx-petalinux-DIRECTORY>/bsp/release
export BSP=xilinx-vck190-v2024.2-final.bsp

# Setup SDKTARGETSYSROOT and CXX...
source $COMMON_IMAGE_VERSAL/environment-setup-cortexa72-cortexa53-xilinx-linux

# Source VITIS ...
source $XILINX_VITIS/settings64.sh

#To source petalinux: 
source <PATH TO petalinux-v2024.2-final>/settings.sh

# =========================================================
# Platform Selection...
# =========================================================
tgt_plat=xilinx_vck190_base_202420_1
export PLATFORM=$PLATFORM_REPO_PATHS/$tgt_plat/$tgt_plat\.xpfm

echo ""
echo "Aiecompiler:"
which aiecompiler

echo ""
echo "Vivado:"
which vivado

echo ""
echo "Vitis:"
which vitis

echo ""
echo "Vitis HLS:"
which vitis_hls
echo ""
