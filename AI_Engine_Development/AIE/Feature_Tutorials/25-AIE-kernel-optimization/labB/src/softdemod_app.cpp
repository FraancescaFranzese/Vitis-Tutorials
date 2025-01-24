//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#include "softdemod_graph.h"
#include "config.h"

softdemod_graph sdg;

int main(void)
{
	sdg.init();
	sdg.run(NRUN);
	sdg.end();

  return 0;
}
