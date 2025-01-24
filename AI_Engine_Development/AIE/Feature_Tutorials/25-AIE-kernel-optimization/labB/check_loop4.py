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

# compute squared magnitude
sqmag = np.square(np.absolute(symdiff))

# compute exponentials
emag = np.exp((-1/1.252968e-01) * sqmag)

# compute exponential sums
esums = np.empty(8*nsym, dtype=float)
for i in range(nsym):
    ev = emag[i*16:(i+1)*16]
    evs = np.empty(8, dtype=float)
    evs[0] = np.sum(ev[0:8])
    evs[1] = np.sum(ev[0:4]) + np.sum(ev[8:12])
    evs[2] = np.sum(ev[0:2]) + np.sum(ev[4:6]) + np.sum(ev[8:10]) + np.sum(ev[12:14])
    evs[3] = np.sum(ev[0::2])
    evs[4] = np.sum(ev[8:16])
    evs[5] = np.sum(ev[4:8]) + np.sum(ev[12:16])
    evs[6] = np.sum(ev[2:4]) + np.sum(ev[6:8]) + np.sum(ev[10:12]) + np.sum(ev[14:16])
    evs[7] = np.sum(ev[1::2])
    esums[i*8:(i+1)*8] = evs;

# read in AIE results
aie_data = np.loadtxt("./x86simulator_output/data/internal_loop_aie.txt", dtype='float', ndmin=2)
aie_data_flat = aie_data.reshape(1,-1).squeeze()

# subtract results
test_diff = np.abs(aie_data_flat - esums)
max_diff = np.max(test_diff)
print("Max error is %f." % (max_diff))

# print results
if max_diff < 0.1:
    print("AIE simulation matches reference data well.")
else:
    print("AIE simulation does not match reference data well.")
    print("You may want to check your kernel code.")
