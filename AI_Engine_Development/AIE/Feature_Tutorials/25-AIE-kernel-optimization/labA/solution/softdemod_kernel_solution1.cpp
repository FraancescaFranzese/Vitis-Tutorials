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
                                 adf::output_buffer<cfloat, extents<16*BUFSZ>>& __restrict loopout)
#endif
{
    //-------------------------------------------------------------------------
    // compute differences between received symbol and reference constellation
    //-------------------------------------------------------------------------
    // Begin section 1A
    //-------------------------------------------------------------------------
    /*
    // load reference constellation
    aie::vector<cfloat,16> ref_const = aie::load_v<16>(constel); 

    auto pIn = aie::begin(in);                              // input buffer
    auto pwbufA16 = aie::begin_restrict_vector<16>(wbufA);  // store differences

    // process all symbols in buffer
    for (unsigned i = 0; i < BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(BUFSZ)
    {
        // compute differences and store in memory buffer
        *pwbufA16++ = aie::sub(ref_const, *pIn++);
    }
    */
    //-------------------------------------------------------------------------
    // End section 1A
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Begin section 1B
    //-------------------------------------------------------------------------
    
    aie::vector<cfloat,4> ref_const;   // holds reference constellation symbols
    auto pIn = aie::begin(in);                   // input buffer
    auto pwbufA4 = aie::begin_vector<4>(wbufA);  // store differences

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
    // End section 1B
    //-------------------------------------------------------------------------   
   // Dump output of previous loop to file if running x86 simulation.
    //-------------------------------------------------------------------------
#ifdef _DUMPLOOP_
    auto pLoopOut = aie::begin_vector<16>(loopout);
    auto pwbufA16D = aie::begin_vector<16>(wbufA);

  for (unsigned i = 0; i < BUFSZ; i++)
		  chess_prepare_for_pipelining
		  chess_loop_count(BUFSZ)
	{
      *pLoopOut++ = *pwbufA16D++;
  }
#endif
    //-------------------------------------------------------------------------    chess_separator();
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
    // Begin section 4
    //-------------------------------------------------------------------------
    // sum exponential components for 1 and 0 bit values according to 
    // constellation mapping
    //-------------------------------------------------------------------------

    for (unsigned i = 0; i < BUFSZ; i++)
		    chess_prepare_for_pipelining
		    chess_loop_count(BUFSZ)
	  {
        aie::vector<float,8> expsum;
        auto expvec1 = *pwbufC8++;
        auto expvec2 = *pwbufC8++;
        auto vsum_L1_1 = fpadd(expvec1,expvec1,0,0x67452301);
        auto vsum_L1_2 = fpadd(expvec2,expvec2,0,0x67452301);
        auto vsum_L1_3 = fpadd(expvec1,expvec2,0,0x76543210);
        auto vsum_L2_1 = fpadd(vsum_L1_1,vsum_L1_1,0,0x54761032);
        auto vsum_L2_2 = fpadd(vsum_L1_2,vsum_L1_2,0,0x54761032);
        auto vsum_L2_3 = fpadd(vsum_L1_3,vsum_L1_3,0,0x67452301);
        auto vsum_L2_4 = fpadd(vsum_L1_3,vsum_L1_3,0,0x54761032);
        aie::vector<float,8> vsum_L3_1 = fpadd(vsum_L2_1,vsum_L2_1,0,0x32107654);
        aie::vector<float,8> vsum_L3_2 = fpadd(vsum_L2_2,vsum_L2_2,0,0x32107654);
        aie::vector<float,8> vsum_L3_3 = fpadd(vsum_L2_3,vsum_L2_3,0,0x54761032);
        aie::vector<float,8> vsum_L3_4 = fpadd(vsum_L2_3,vsum_L2_3,0,0x32107654);
        aie::vector<float,8> vsum_L3_5 = fpadd(vsum_L2_4,vsum_L2_4,0,0x32107654);
        expsum[0] = vsum_L3_1[0];
        expsum[4] = vsum_L3_2[0];
        expsum[1] = vsum_L3_3[0];
        expsum[5] = vsum_L3_3[4];        
        expsum[2] = vsum_L3_4[0];
        expsum[6] = vsum_L3_4[2];
        expsum[3] = vsum_L3_5[0];
        expsum[7] = vsum_L3_5[1];
        *pwbufB8++ = expsum;
    }
 
     pwbufB8 -= BUFSZ;  // reset iterator for next loop

    //-------------------------------------------------------------------------
    // End section 4
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