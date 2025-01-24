#!/usr/bin/env python3
#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Richard Buz

import re
import numpy as np

apsk16 = np.array([0.8031182+0.8031182j,0.8031182-0.8031182j,-0.8031182+0.8031182j,-0.8031182-0.8031182j, \
          1.0970798+0.2939616j,1.0970798-0.2939616j,-1.0970798+0.2939616j,-1.0970798-0.2939616j, \
          0.2939616+1.0970798j,0.2939616-1.0970798j,-0.2939616+1.0970798j,-0.2939616-1.0970798j, \
          0.2549582+0.2549582j,0.2549582-0.2549582j,-0.2549582+0.2549582j,-0.2549582-0.2549582j])

# read complex-valued symbols from text file
data_in = np.loadtxt("./data/input_softdemod.txt", dtype='float', ndmin=2)
nsym = data_in.shape[0]
rxsym = np.empty(nsym, dtype=complex)
rxsym.real = data_in[:,0]
rxsym.imag = data_in[:,1]

# compute symbol differences
symdiff = np.empty(16*nsym, dtype=complex)
for i in range(rxsym.size):
    symdiff[i*16:(i+1)*16] = apsk16 - rxsym[i]

# read in AIE results
aie_data = np.loadtxt("./x86simulator_output/data/internal_loop_aie.txt", dtype='float', ndmin=2)
nsmp = aie_data.shape[0]
simdata = np.empty(nsmp, dtype=complex)
simdata.real = aie_data[:,0]
simdata.imag = aie_data[:,1]

# subtract results
test_diff = np.abs(simdata - symdiff)
max_diff = np.max(test_diff)
print("Max error is %e." % (max_diff))

# print results
if max_diff < 1.0e-5:
    print("AIE simulation matches reference data well.")
else:
    print("AIE simulation does not match reference data well.")
    print("You may want to check your kernel code.")
