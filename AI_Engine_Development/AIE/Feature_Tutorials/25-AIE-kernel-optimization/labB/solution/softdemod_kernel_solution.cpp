//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#include "softdemod_kernel.h"
#include <adf.h>
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>

using namespace adf;

void softdemod_kernel::softdemod(adf::input_buffer<cfloat, extents<BUFSZ>>& __restrict in, 
#ifndef _DUMPLOOP_
                                 adf::output_buffer<float, extents<4*BUFSZ>>& __restrict out)
#else
                                 adf::output_buffer<float, extents<4*BUFSZ>>& __restrict out,
                                 adf::output_buffer<float, extents<8*BUFSZ>>& __restrict loopout)
#endif
{
    //-------------------------------------------------------------------------
    // compute differences between received symbol and reference constellation
    //-------------------------------------------------------------------------
    // Begin section 1
    //-------------------------------------------------------------------------
    
    aie::vector<cfloat,4> ref_const;   // holds reference constellation symbols
    auto pIn = aie::begin(in);                            // input buffer
    auto pwbufA4 = aie::begin_restrict_vector<4>(wbufA);  // store differences

    // process all symbols in buffer, one at a time
    for (unsigned i = 0; i < BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(BUFSZ)
    {
		// get received symbol
		auto rxsym = *pIn++;

        // 16-point constellation, compute 4 differences at a time
        ref_const = aie::load_v<4>(&constel[0]);
        *pwbufA4++ = aie::sub(ref_const, rxsym);
        ref_const = aie::load_v<4>(&constel[4]);
        *pwbufA4++ = aie::sub(ref_const, rxsym);
        ref_const = aie::load_v<4>(&constel[8]);
        *pwbufA4++ = aie::sub(ref_const, rxsym);
        ref_const = aie::load_v<4>(&constel[12]);
        *pwbufA4++ = aie::sub(ref_const, rxsym);
    }
    
    //-------------------------------------------------------------------------
    // End section 1
    //-------------------------------------------------------------------------   
    chess_separator();
    //-------------------------------------------------------------------------
    // Begin section 2
    //-------------------------------------------------------------------------
    // compute magnitude squared of the differences between symbols
    //-------------------------------------------------------------------------

    auto pwbufA8 = aie::begin_restrict_vector<8>(wbufA);  // input differences
    auto pwbufB8 = aie::begin_restrict_vector<8>(wbufB);  // output squared differences

    // process 8 symbols at a time
    for (unsigned i = 0; i < 2*BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(2*BUFSZ)
	{
        *pwbufB8++ = aie::abs_square(*pwbufA8++);
    }

    pwbufB8 -= 2*BUFSZ;  // reset iterator for next loop

    //-------------------------------------------------------------------------
    // End section 2
    //-------------------------------------------------------------------------
    chess_separator();
    //-------------------------------------------------------------------------
    // Begin section 3
    //-------------------------------------------------------------------------
    // IEEE 754 format trick to estimate exponential function
    //-------------------------------------------------------------------------

    aie::accum<accfloat,8> acc_init;    // add a constant to all vector lanes
    acc_init.from_vector(aie::broadcast<float,8>(EADD),0); 
    auto pwbufC8 = aie::begin_restrict_vector<8>(wbufC);  //store results

    // transform input arguement to obtain estimate of floating point exponent
    for (unsigned i = 0; i < 2*BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(2*BUFSZ)
	{
        *pwbufC8++ = aie::mac(acc_init, *pwbufB8++, EMULT);
    }

    pwbufB8 -= 2*BUFSZ;  // reset iterators for next loop
    pwbufC8 -= 2*BUFSZ;

    chess_separator();

    // compute exponential function estimates
    for (unsigned i = 0; i < 2*BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(2*BUFSZ)
	{
        aie::vector<int32,8> expint = aie::to_fixed(*pwbufC8++, 16);
        *pwbufB8++ = expint.cast_to<float>();
    }

    pwbufB8 -= 2*BUFSZ;  // reset iterators for next loop
    pwbufC8 -= 2*BUFSZ;

    chess_separator();

    // check for numerical issues and set values to zero if necessary
    for (unsigned i = 0; i < 2*BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(2*BUFSZ)
	{
        aie::mask<8> msk_neg = aie::lt(*pwbufB8, 0.0f);
        *pwbufC8++ = aie::select(*pwbufB8++, 0.0f, msk_neg);
    }

    pwbufB8 -= 2*BUFSZ;  // reset iterators for next loop
    pwbufC8 -= 2*BUFSZ;

    //-------------------------------------------------------------------------
    // End section 3
    //-------------------------------------------------------------------------
    chess_separator();
    //-------------------------------------------------------------------------
    // Begin section 4A
    //-------------------------------------------------------------------------
    // sum exponential components for 1 and 0 bit values according to 
    // constellation mapping
    //-------------------------------------------------------------------------
    /*
    auto pwbufC16 = aie::begin_restrict_vector<16>(wbufC); // input exponentials

    for (unsigned i = 0; i < BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(BUFSZ)
	{
        aie::vector<float,8> expsum;  // holds exponentials sums
        
        expsum[0] = aie::reduce_add(aie::filter_even(*pwbufC16,  8));
        expsum[1] = aie::reduce_add(aie::filter_even(*pwbufC16,  4));
        expsum[2] = aie::reduce_add(aie::filter_even(*pwbufC16,  2));
        expsum[3] = aie::reduce_add(aie::filter_even(*pwbufC16,  1));
        expsum[4] = aie::reduce_add(aie::filter_odd( *pwbufC16,  8));
        expsum[5] = aie::reduce_add(aie::filter_odd( *pwbufC16,  4));
        expsum[6] = aie::reduce_add(aie::filter_odd( *pwbufC16,  2));
        expsum[7] = aie::reduce_add(aie::filter_odd( *pwbufC16++,1));
        
        *pwbufB8++ = expsum;
    }
 
     pwbufB8 -= BUFSZ;  // reset iterator for next loop
    */
    //-------------------------------------------------------------------------
    // End section 4A
    //-------------------------------------------------------------------------    
    //-------------------------------------------------------------------------
    // Begin section 4B
    //-------------------------------------------------------------------------
    // sum exponential components for 1 and 0 bit values according to 
    // constellation mapping
    //-------------------------------------------------------------------------

    auto pwbufC16 = aie::begin_restrict_vector<16>(wbufC); // input exponentials

    for (unsigned i = 0; i < BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(BUFSZ)
	{
        aie::vector<float,8> expsum = aie::zeros<float,8>(); // accumulator register
        
        expsum = fpadd(expsum,*pwbufC16,0,0x12480000);
        expsum = fpadd(expsum,*pwbufC16,0,0x33592111);
        expsum = fpadd(expsum,*pwbufC16,0,0x566A4422);
        expsum = fpadd(expsum,*pwbufC16,0,0x777B6533);
        expsum = fpadd(expsum,*pwbufC16,0,0x9ACC8884);
        expsum = fpadd(expsum,*pwbufC16,0,0xBBDDA995);
        expsum = fpadd(expsum,*pwbufC16,0,0xDEEECCA6);
        expsum = fpadd(expsum,*pwbufC16++,0,0xFFFFEDB7);
        
        *pwbufB8++ = expsum;
    }
 
     pwbufB8 -= BUFSZ;  // reset iterator for next loop

    //-------------------------------------------------------------------------
    // End section 4B
    //-------------------------------------------------------------------------
    // Dump output of previous loop to file if running x86 simulation.
    //-------------------------------------------------------------------------
#ifdef _DUMPLOOP_
    auto pLoopOut = aie::begin_vector<8>(loopout);

    for (unsigned i = 0; i < BUFSZ; i++)
		chess_prepare_for_pipelining
		chess_loop_count(BUFSZ)
	{
        *pLoopOut++ = *pwbufB8++;
    }

    pwbufB8 -= BUFSZ;
#endif
    //-------------------------------------------------------------------------
     chess_separator();
    //-------------------------------------------------------------------------
    // Begin section 5
    //-------------------------------------------------------------------------
    // compute likelihood ratios
    //-------------------------------------------------------------------------

    auto pwbufC4 = aie::begin_restrict_vector<4>(wbufC);  // store results

    for (unsigned i = 0; i < BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(BUFSZ)
	{
        aie::vector<float,8> expsum = *pwbufB8++;
        *pwbufC4++ = aie::mul(expsum.extract<4>(0),inv(expsum.extract<4>(1)));
    }
	
    pwbufC4 -= BUFSZ;  // reset iterator for next loop

    //-------------------------------------------------------------------------
    // End section 5
    //-------------------------------------------------------------------------
    chess_separator();
    //-------------------------------------------------------------------------
    // Begin section 6
    //-------------------------------------------------------------------------
    // No logarithm implemented yet. Send likelihood ratios to output.
    //-------------------------------------------------------------------------

    auto pOut = aie::begin_vector<4>(out);

    for (unsigned i = 0; i < BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(BUFSZ)
	  {
		*pOut++ = *pwbufC4++;
	  }

    //-------------------------------------------------------------------------
    // End section 6
    //-------------------------------------------------------------------------
}