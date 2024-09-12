#!/bin/bash

# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# Author: Daniele Bagni
# Date:   03 Sep 2024

######################################################################################
# WARNING: THIS SCRIPT IS VALID ONLY FOR VCK190 on AMD/XILINX GUUP (LINUX) SERVERS
######################################################################################

# =======================================================
# GUUP Environment Setup:
# set Platform, Vitis and Petalinux repo
# =======================================================
#ts 2024.2_daily_latest -petalinux petalinux-v2024.2_daily_latest
#ts 2024.2_released     -petalinux petalinux-v2024.2_daily_latest


export CUR_DIR=$PWD

# =======================================================
# global variables for this tutorial
# =======================================================
#RELEASE=
RELEASE=2024.2
BOARD=vck190
BASE_NUM=202420_1
#BASE_NUM=202420_1

export WRK_DIR=/group/xirguup/danieleb/ide2024.2/Vitis-Tutorials/Developer_Contributed
export TUTORIAL=03-HLS_Code_Optimization


# =======================================================
# to emulate SPRITE regression env in GUUP Servers
# =======================================================
#cd ${WRK_DIR}/../Vitis-Tutorials-Admin-2024.2_next
source ./env_setup_petalinux_versal.sh
##back to the wrk dir
#cd ${WRK_DIR}/${TUTORIAL}/files/

#==================================================================================
# Set Versal Common Image repo (OR eventually install it into /tmp)
# This local foldr must not be a "NFS" hard disk drive, only "ext4" is supported
# ==================================================================================

# export TMP DIRECTORY=<YOUR-EXT4-TMP-(NO-NFS)-DIRECTORY>
mkdir -p /tmp/danieleb
export TMPDIR=/tmp/danieleb

# PETALINUX and VERSAL COMMON IMAGE
export COMMON_IMAGE_VERSAL=${HOME}/common/xilinx-versal-common-v2024.2

: '
# do it only once
# clean versal folder content
rm -rf ${COMMON_IMAGE_VERSAL}
# install SDK
export VERSAL_COMMON_2023v2=$PETALINUX/../../bsp/internal/xilinx-versal-common-v2024.2/
cd ${VERSAL_COMMON_2023v2}
bash ./sdk.sh -y -p -d ${COMMON_IMAGE_VERSAL}
cp bl31.elf boot.scr Image README.txt rootfs.ext4 rootfs.manifest u-boot.elf ${COMMON_IMAGE_VERSAL}/
'
cd ${WRK_DIR}/${TUTORIAL}/files


# ========================================================
# Set DSP Library for Vitis
# ========================================================
## already set in "env_setup.sh" but not yet available in EMEA servers
#export DSPLIB_ROOT=<Path to 2024.2 DSP Libs - Directory>
export DSPLIB_VITIS=/group/xirguup/danieleb/ide2024.2/Vitis_Libraries
#export DSPLIB_VITIS=/group/xirguup/danieleb/ide2022.2/Vitis_Libraries
export DSPLIB_ROOT=${DSPLIB_VITIS}/dsp


# =========================================================
# Platform Selection...
# =========================================================
export VERSAL_VITIS_PLATFORM=xilinx_${BOARD}\_base_${BASE_NUM}
export VITIS_PLATFORM_DIR=${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM=${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm

# ========================================================
# Set SysRoot, RootFS and Image
# ========================================================
export VITIS_SYSROOTS=${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux
export SDKTARGETSYSROOT=${VITIS_SYSROOTS}
export KERNEL_IMAGE=${COMMON_IMAGE_VERSAL}/Image
export ROOTFS=${COMMON_IMAGE_VERSAL}/rootfs.ext4

# ========================================================
# for compatibility with official Vitis Tutorials
# ========================================================
export XLNX_VERSAL=${COMMON_IMAGE_VERSAL}
export PLATFORM=${VITIS_PLATFORM_XPFM}

# ========================================================
# check env variables
# ========================================================
echo " "
echo "Vitis/Petalinux GUUP Environmental variables"
echo " "
echo "LD_LIBRARY_PATH     "  $LD_LIBRARY_PATH
echo "XILINX_VITIS        "  $XILINX_VITIS
echo "BSP_DIR             "  $BSP_DIR
#echo "PETALINUX_VER       "  $PETALINUX_VER
echo "PLATFORM_REPO_PATHS "  $PLATFORM_REPO_PATHS
echo "XILINX_XRT          "  $XILINX_XRT
echo "XILINX_VIVADO       "  $XILINX_VIVADO
#echo "XILINXD_LICENSE_FILE " $XILINXD_LICENSE_FILE
echo "XILINX_HLS          "  $XILINX_HLS
#echo "XSCT_TOOLCHAIN"        $XSCT_TOOLCHAIN
#echo "XDG_DATA_DIRS "        $XDG_DATA_DIRS
#echo "GUUP_ROOT "            $GUUP_ROOT
echo "SHELL               "   $SHELL
#echo "LM_LICENSE_FILE "      $LM_LICENSE_FILE
#echo "PYTHONPATH "           $PYTHONPATH
#echo "PETALINUX_MAJOR_VER "  $PETALINUX_MAJOR_VER
echo "PETALINUX            "  $PETALINUX
echo "SDKTARGETSYSROOT     "  ${VITIS_SYSROOTS}
echo "VERSAL_COMMON_2023v1 "  ${VERSAL_COMMOM_2023v1}
echo "COMMON_IMAGE_VERSAL  "  ${COMMON_IMAGE_VERSAL}
echo "SDKTARGETSYSROOT     "  ${VITIS_SYSROOTS}
echo "VITIS_PLATFORM_XPFM  "  ${VITIS_PLATFORM_XPFM}
echo "KERNEL_IMAGE         "  ${KERNEL_IMAGE}
echo "ROOTFS               "  ${ROOTFS}
echo "VITIS_PLATFORM_XPFM  "  ${VITIS_PLATFORM_XPFM}
echo "PLATFORM_REPO_PATHS  "  ${PLATFORM_REPO_PATHS}
#echo "PATH " ${PATH}
echo "DSPLIB_ROOT          "  ${DSPLIB_VITIS}/dsp
