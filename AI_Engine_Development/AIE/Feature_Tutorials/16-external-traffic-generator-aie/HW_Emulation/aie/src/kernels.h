/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
#ifndef __KERNELS_H__
#define __KERNELS_H__

//#include <adf/window/types.h>
//#include <adf/stream/types.h>
#include <adf.h>
#include <aie_api/aie.hpp>
using namespace adf;
 void classifier(input_stream_cint16 * input, 
                  output_buffer<int32> & __restrict outputw);

  void fir_27t_sym_hb_2i(       
	input_buffer<cint16,adf::extents<adf::inherited_extent>,adf::margin<INTERPOLATOR27_COEFFICIENTS>>  & __restrict cb_input,
	output_buffer<cint16> & __restrict cb_output);
			
#endif /**********__KERNELS_H__**********/
