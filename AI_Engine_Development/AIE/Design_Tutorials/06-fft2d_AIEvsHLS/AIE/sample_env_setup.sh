#!/bin/bash

#Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

# =======================================================
# Set Platform ,Vitis and Versal Image repo
# =======================================================
export PLATFORM_REPO_PATHS= <YOUR-2024.2-PLATFORM-DIRECTORY>
export XILINX_VITIS = <YOUR-2024.2-VITIS-DIRECTORY>/2024.2
export COMMON_IMAGE_VERSAL=<YOUR-XILINX-VERSAL-common-v2024.2-DIRECTORY>
# ====================================================
# Source Versal Image ,Vitis and Aietools
# ====================================================
# Run the below command to setup environment and CXX
source $COMMON_IMAGE_VERSAL/environment-setup-cortexa72-cortexa53-xilinx-linux
source $XILINX_VITIS/settings64.sh
# ========================================================
# Set DSP Library for Vitis
# ========================================================
export DSPLIB_VITIS=<Path to Vitis Libs - Directory>
# =========================================================
# Platform Selection...
# =========================================================
tgt_plat=xilinx_vck190_base_202420_1
export PLATFORM=$PLATFORM_REPO_PATHS/$tgt_plat/$tgt_plat\.xpfm
# ==========================================================
# Validating Tool Installation
# ==========================================================
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
echo ""
echo "DSPLIBS"
echo "$DSPLIB_VITIS"
echo ""
