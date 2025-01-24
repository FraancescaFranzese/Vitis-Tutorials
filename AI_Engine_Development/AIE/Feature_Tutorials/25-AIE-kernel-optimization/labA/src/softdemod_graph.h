//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#pragma once

#include <adf.h>
#include "softdemod_kernel.h"
#include "config.h"
#include "mod_const.h"

using namespace adf;

class softdemod_graph : public adf::graph {
private:
  kernel softdemod_krnl;

public:
  input_plio iplio;
  output_plio oplio;
#ifdef _DUMPLOOP_
  output_plio loopplio;
#endif

  softdemod_graph()
  {
      iplio = input_plio::create("DIN", plio_64_bits, "data/input_softdemod.txt", 625.0);
      oplio = output_plio::create("DOUT", plio_64_bits, "data/output_softdemod_aie.txt", 625.0);
#ifdef _DUMPLOOP_
      loopplio = output_plio::create("TSTOUT", plio_64_bits, "data/internal_loop_aie.txt", 625.0);
#endif

      softdemod_krnl = kernel::create_object<softdemod_kernel>(std::vector<cfloat>{APSK16},std::vector<cfloat>(16*BUFSZ),
                       std::vector<float>(16*BUFSZ), std::vector<float>(16*BUFSZ));

      connect(iplio.out[0], softdemod_krnl.in[0]);
      connect(softdemod_krnl.out[0], oplio.in[0]);
#ifdef _DUMPLOOP_
      connect(softdemod_krnl.out[1], loopplio.in[0]);
#endif

      source(softdemod_krnl) = "src/softdemod_kernel.cpp";

      runtime<ratio>(softdemod_krnl) = 0.9;
  }
};

