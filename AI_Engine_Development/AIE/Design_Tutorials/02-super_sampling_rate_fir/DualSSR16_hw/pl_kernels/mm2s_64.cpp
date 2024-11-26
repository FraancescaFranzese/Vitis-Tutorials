/*
Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>


extern "C" {

	void mm2s_64(ap_int<64>* mem, hls::stream<qdma_axis<64, 0, 0, 0>  >& s, int size) {
		#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem

		#pragma HLS interface axis port=s

		#pragma HLS INTERFACE s_axilite port=mem bundle=control
		#pragma HLS INTERFACE s_axilite port=size bundle=control
		#pragma HLS interface s_axilite port=return bundle=control
		ap_int<64> v[512];

		for(int i=0;i<512;i++)
		{
			#pragma HLS PIPELINE II=1
			v[i] = mem[i];
		}

		for(int i = 0; i < size; i++) {
			#pragma HLS PIPELINE II=1
			qdma_axis<64, 0, 0, 0> x;
			x.data = v[i%512];
			x.last = ((i%256)==255);

			x.keep_all();
			s.write(x);
		}

}
