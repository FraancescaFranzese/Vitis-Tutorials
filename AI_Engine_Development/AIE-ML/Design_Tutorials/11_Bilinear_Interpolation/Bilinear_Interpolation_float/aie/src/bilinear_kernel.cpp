//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#include "bilinear_kernel.h"
#include "aie_api/accum.hpp"
#include "aie_api/vector.hpp"
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>

void bilinear_kernel::interp(input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_A, 
                             input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_B, 
                             input_buffer<int32, extents<BUFFER_SIZE_IN>>& __restrict in_C, 
                             output_buffer<int32, extents<BUFFER_SIZE_OUT>>& __restrict out)
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
		aie::vector<float, 8> xfrac = (*pInA++).cast_to<float>();
		aie::vector<float, 8> p11 = (*pInB++).cast_to<float>();
		aie::vector<float, 8> p21 = (*pInC++).cast_to<float>();

        aie::accum<accfloat, 8> p11_acc;
        p11_acc.from_vector(p11);

		// compute first x interpolation
		aie::accum<accfloat, 8> tempy1 = aie::mac(p11_acc,xfrac,p21); //fpmac(p11,xfrac,0,0x76543210,p21,0,0x76543210);
        p11= p11_acc.to_vector();
		aie::accum<accfloat, 8> pxy1 = aie::msc(tempy1, xfrac, p11); //fpmsc(tempy1,xfrac,0,0x76543210,p11,0,0x76543210);

		// get data for second x interpolation
		aie::vector<float, 8> p12 = (*pInB++).cast_to<float>();
		aie::vector<float, 8> p22 = (*pInC++).cast_to<float>();

        aie::accum<accfloat, 8> p12_acc;
        p12_acc.from_vector(p12);

		// compute second x interpolation
		aie::accum<accfloat, 8> tempy2 = aie::mac(p12_acc,xfrac,p22);//fpmac(p12,xfrac,0,0x76543210,p22,0,0x76543210);
        p12 = p12_acc.to_vector();
		aie::accum<accfloat, 8> pxy2 = msc(tempy2,xfrac,p12);//fpmsc(tempy2,xfrac,0,0x76543210,p12,0,0x76543210);        

		// get data for y interpolation
		aie::vector<float, 8> yfrac = (*pInA++).cast_to<float>();


		// compute y interpolation
		aie::accum<accfloat, 8> tempxy = aie::mac(pxy1,yfrac,pxy2.to_vector());//fpmac(pxy1,yfrac,0,0x76543210,pxy2,0,0x76543210);
		aie::accum<accfloat, 8> pxy = aie::msc(tempxy, yfrac,pxy1.to_vector()); //fpmsc(tempxy,yfrac,0,0x76543210,pxy1,0,0x76543210);

		// write interpolated pixels to output
		*pOut++ = aie::vector_cast<int32>(pxy.to_vector());


	}
}
