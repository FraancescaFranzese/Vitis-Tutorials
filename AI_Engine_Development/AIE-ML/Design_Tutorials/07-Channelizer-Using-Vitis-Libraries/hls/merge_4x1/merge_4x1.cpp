//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "merge_4x1.h"
using namespace merge_4x1;

// ------------------------------------------------------------
// Consumer
// ------------------------------------------------------------

void consumer( TT_STREAM sig_i[NSTREAM_I], hls::stream_of_blocks<TT_BLOCK>& ss )
{
#pragma HLS pipeline II=8
  hls::write_lock<TT_BLOCK> WL(ss);
  WL[ 0] = sig_i[ 0].read();
  WL[ 1] = sig_i[ 1].read();
  WL[ 2] = sig_i[ 2].read();
  WL[ 3] = sig_i[ 3].read();
}

// ------------------------------------------------------------
// Producer
// ------------------------------------------------------------

void producer( hls::stream_of_blocks<TT_BLOCK>& ss, TT_STREAM& sig_o )
{
#pragma HLS pipeline II=8
  // We produce 8 samples over 8 cycles (two cint32 @ 312.5 MHz)
  // Inputs arrive in polyphase order:
  hls::read_lock<TT_BLOCK> RL(ss);
  TT_SAMPLE v00, v04;    (v04,v00) = RL[ 0];
  TT_SAMPLE v01, v05;    (v05,v01) = RL[ 1];
  TT_SAMPLE v02, v06;    (v06,v02) = RL[ 2];
  TT_SAMPLE v03, v07;    (v07,v03) = RL[ 3];
  // Restore linear order:
  sig_o.write( (v01,v00) );
  sig_o.write( (v03,v02) );
  sig_o.write( (v05,v04) );
  sig_o.write( (v07,v06) );
}

// ------------------------------------------------------------
// Wrapper
// ------------------------------------------------------------

void merge_4x1_wrapper( TT_STREAM sig_i[NSTREAM_I], TT_STREAM& sig_o )
{
#pragma HLS interface mode=ap_ctrl_none port=return
#pragma HLS interface axis       port=sig_i
#pragma HLS interface axis       port=sig_o

  hls::stream_of_blocks<TT_BLOCK> ss;
#pragma HLS bind_storage variable=ss type=ram_s2p impl=lutram

#pragma HLS dataflow

  consumer( sig_i, ss );
  producer( ss, sig_o );
}



