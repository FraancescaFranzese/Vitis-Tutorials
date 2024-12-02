//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

// Modified by: Francesca Franzese (for aie-ml execution)

#include "bilinear_kernel.h"
#include "aie_api/accum.hpp"
#include "aie_api/types.hpp"
#include "aie_api/vector.hpp"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>

void bilinear_kernel::interp(input_buffer<int16, extents<BUFFER_SIZE_IN>>& __restrict in_A, 
                             input_buffer<int16, extents<BUFFER_SIZE_IN>>& __restrict in_B, 
                             input_buffer<int16, extents<BUFFER_SIZE_IN>>& __restrict in_C, 
                             output_buffer<int16, extents<BUFFER_SIZE_OUT>>& __restrict out)
{
	// iterators for input & output buffers
	auto pInA = aie::begin_vector<8>(in_A);
	auto pInB = aie::begin_vector<8>(in_B);
	auto pInC = aie::begin_vector<8>(in_C);
	auto pOut = aie::begin_vector<8>(out);

    for (unsigned i = 0; i < PXLPERGRP/8; i++)
		chess_prepare_for_pipelining
		chess_loop_count(PXLPERGRP/8)
	{
		// get data for first x interpolation
		aie::vector<int16,8> xfrac = *pInA++;
		aie::vector<int16,8> p11 = *pInB++;
		aie::vector<int16,8> p21 = *pInC++;

        	aie::accum<acc32, 8> p11_acc;
        	p11_acc.from_vector(p11);

		// compute first x interpolation
		auto tempy1 = mac(p11_acc,xfrac,p21);
		auto pxy1 = msc(tempy1,xfrac,p11);

		// get data for second x interpolation
		aie::vector<int16,8> p12 = *pInB++;
		aie::vector<int16,8> p22 = *pInC++;

        	aie::accum<acc32, 8> p12_acc;
       		p12_acc.from_vector(p12);

		// compute second x interpolation
		auto tempy2 = mac(p12_acc,xfrac,p22);
		auto pxy2 = msc(tempy2,xfrac,p12);

		// get data for y interpolation
		aie::vector<int16,8> yfrac = *pInA++;

        	aie::vector<int16, 8> pxy2_v;
        	pxy2_v = pxy2.to_vector<int16>();
        
        	aie::vector<int16, 8> pxy1_v;
        	pxy1_v = pxy2.to_vector<int16>();


		// compute y interpolation
		auto tempxy = mac(pxy1,yfrac,pxy2_v);
		auto pxy = msc(tempxy,yfrac,pxy1_v);


		*pOut++ = pxy.to_vector<int16>();
	}
}
