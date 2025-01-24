//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>
#include "config.h"

using namespace adf;

class softdemod_kernel
{
    private:
        alignas(16) cfloat (&constel)[16];

        alignas(16) cfloat (&wbufA)[16*BUFSZ];
        alignas(16) float (&wbufB)[16*BUFSZ];
        alignas(16) float (&wbufC)[16*BUFSZ];

        static constexpr float EMULT = (-1.0/NVAR)*184.6649652337873;
        static constexpr float EADD = 16249.0;

    public:
        softdemod_kernel(cfloat (&constel_i)[16], cfloat (&wbufA_i)[16*BUFSZ], float (&wbufB_i)[16*BUFSZ], float (&wbufC_i)[16*BUFSZ]) 
                : constel(constel_i), wbufA(wbufA_i), wbufB(wbufB_i), wbufC(wbufC_i) {};
        void softdemod(adf::input_buffer<cfloat, extents<BUFSZ>>& in, 
#ifndef _DUMPLOOP_
                     adf::output_buffer<float, extents<4*BUFSZ>>& out);
#else
                     adf::output_buffer<float, extents<4*BUFSZ>>& out,
                     adf::output_buffer<cfloat, extents<16*BUFSZ>>& loopout);
#endif

	    static void registerKernelClass()
  	    {
		    REGISTER_FUNCTION(softdemod_kernel::softdemod);
            REGISTER_PARAMETER(constel);
            REGISTER_PARAMETER(wbufA);
            REGISTER_PARAMETER(wbufB);
            REGISTER_PARAMETER(wbufC);
	    }
};
