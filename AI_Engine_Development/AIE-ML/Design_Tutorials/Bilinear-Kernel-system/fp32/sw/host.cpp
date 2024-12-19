
//
// Author: Francesca Franzese
//

// Instantiate AIE graph:
#include "../aie/src/config.h"

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <adf.h>
#include <input_1_A.h>
#include <input_1_B.h>
#include <input_1_C.h>
#include <output_1_ref.h>

#include <experimental/xrt_xclbin.h>
#include <experimental/xrt_kernel.h>
#include <experimental/xrt_device.h>
#include <experimental/xrt_ip.h>
#include <experimental/xrt_graph.h>

#include <experimental/xrt_aie.h>
#include <experimental/xrt_graph.h>
#include <experimental/xrt_ip.h>

#define N_ITERATION 4096
#define INPUT_SIZE (2*PXLPERGRP)
#define OUTPUT_SIZE PXLPERGRP

// ------------------------------------------------------------
// DDR Parameters
// ------------------------------------------------------------

static constexpr unsigned            DEPTH = 1024; // 4 graph iterations x 1024 / 4 samples @ 128-bit
static constexpr unsigned DDR_WORD_DEPTH_I = DEPTH;
static constexpr unsigned DDR_WORD_DEPTH_O = DEPTH;
static constexpr unsigned    NUM_SAMPLES_I = 1024 * 1024 * 2; // input: 1024*1024 couples of int16 data
static constexpr unsigned    NUM_SAMPLES_O = 1024 * 1024; // output: 1024*1024 couples of int16 data

static constexpr unsigned DDR_BUFFSIZE_I_BYTES = NUM_SAMPLES_I * sizeof(int32); // Each sample is 2 bytes (16-bits)
static constexpr unsigned DDR_BUFFSIZE_O_BYTES = NUM_SAMPLES_O * sizeof(int32); // Each sample is 2 bytes (16-bits)

using namespace adf;
using namespace std;


int main(int argc, char* argv[]) {
	

	//TARGET_DEVICE macro needs to be passed from gcc command line
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return 1;
	}

	//////////////////////////////////////////
	// Open device and load xclbin
	//////////////////////////////////////////	
	
	char* xclbinFilename = argv[1];
	unsigned dev_index = 0;
	auto my_device = xrt::device(dev_index);
	if(!my_device){
        std::cout << "Device open error!" << std::endl;
    }else{
		std::cout << "Device open OK!" << std::endl;
	}

	auto xclbin_uuid = my_device.load_xclbin(xclbinFilename);

    //////////////////////////////////////////
	// allocating input and output memory
	//////////////////////////////////////////

	auto in_bo1 = xrt::bo(my_device, DDR_BUFFSIZE_I_BYTES, 0);
	auto in_bo1_mapped = in_bo1.map<uint32_t*>();
    memcpy(in_bo1_mapped, v_input_A, DDR_BUFFSIZE_I_BYTES);
    in_bo1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    std::cout << "Input memory virtual address buffer A: " << in_bo1_mapped << std::endl;   
	
	auto in_bo2 = xrt::bo(my_device, DDR_BUFFSIZE_I_BYTES, 0);
	auto in_bo2_mapped = in_bo2.map<uint32_t*>();
    memcpy(in_bo2_mapped, v_input_B, DDR_BUFFSIZE_I_BYTES);
    in_bo2.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    std::cout << "Input memory virtual address buffer B: " << in_bo2_mapped << std::endl;   
	
	auto in_bo3 = xrt::bo(my_device, DDR_BUFFSIZE_I_BYTES, 0);
	auto in_bo3_mapped = in_bo3.map<uint32_t*>();
    memcpy(in_bo3_mapped, v_input_C, DDR_BUFFSIZE_I_BYTES);
    in_bo3.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    std::cout << "Input memory virtual address buffer C: " << in_bo3_mapped << std::endl;   

	auto out_bo = xrt::bo(my_device, DDR_BUFFSIZE_O_BYTES, 0);
    auto out_bo_mapped = out_bo.map<uint32_t*>(); //puntatore alla memoria
	memset(out_bo_mapped, 0x0000, DDR_BUFFSIZE_O_BYTES);   //spazio di memoria inizializzato a 0
    std::cout << "Output memory virtual address buffer: " << out_bo_mapped << std::endl; 

	std::cout << "Memory allocated for input and output buffer" << std::endl;

    	
    //////////////////////////////////////////
	// start input and output kernels
	//////////////////////////////////////////
	
	auto s2mm_k = xrt::kernel(my_device, xclbin_uuid, "s2mm:{s2mm_1}");
	auto s2mm_r = s2mm_k(out_bo, nullptr, DDR_BUFFSIZE_O_BYTES/sizeof(int64));
	std::cout << "s2mm run started" << std::endl;

	auto mm2s_k1 = xrt::kernel(my_device, xclbin_uuid, "mm2s:{mm2s_1}");
	auto mm2s_r1 = mm2s_k1(in_bo1,  nullptr, DDR_BUFFSIZE_I_BYTES/sizeof(int64));
	std::cout << "mm2s1 run started" << std::endl;
	
	auto mm2s_k2 = xrt::kernel(my_device, xclbin_uuid, "mm2s:{mm2s_2}");
	auto mm2s_r2 = mm2s_k2(in_bo2,  nullptr, DDR_BUFFSIZE_I_BYTES/sizeof(int64));
	std::cout << "mm2s2 run started" << std::endl;
	
	auto mm2s_k3 = xrt::kernel(my_device, xclbin_uuid, "mm2s:{mm2s_3}");
	auto mm2s_r3 = mm2s_k3(in_bo3,  nullptr, DDR_BUFFSIZE_I_BYTES/sizeof(int64));
	std::cout << "mm2s3 run started" << std::endl;
	
	std::cout << "HLS Kernel run start" << std::endl;
	

    //////////////////////////////////////////
	//  Load and run AIE graph
	//////////////////////////////////////////

    std::cout << "Allocating aie graph..." << std::endl;
	auto my_graph  = xrt::graph(my_device, xclbin_uuid, "blint");
    std::cout << "Aie graph allocation completed" << std::endl;

	my_graph.reset();
    std::cout << "Running graph for " << NRUN << " iterations" << std::endl;
	my_graph.run(NRUN);
    std::cout << "Waiting for completion..." << std::endl;
    
    my_graph.end();
    std::cout << "Completed!" << std::endl;
	

    	//////////////////////////////////////////
	// Wait for kernels completion
	//////////////////////////////////////////

    std::cout << std::endl << "Waiting for kernels to end..." << std::endl;
	mm2s_r1.wait();
	mm2s_r2.wait();
	mm2s_r3.wait();
    
    s2mm_r.wait();
    out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    std::cout << std::endl << "Kernel execution completed!" << std::endl;

    //////////////////////////////////////////
	// Data validation
	//////////////////////////////////////////
	
    std::cout << std::endl << "Verification of output data with respect to golden reference:" << std::endl;
	
    float max_diff=0;
    float diff=0;
    int32 mismatch_count=0;
    float slack = 1;

    for (unsigned ss=0; ss < DDR_BUFFSIZE_O_BYTES/sizeof(int32); ss++) {

        int32 data_golden;
        int32 data_aie;
        
        float data_golden_fp;
        float data_aie_fp;
        
       	data_aie = (int32)out_bo_mapped[ss];
       	data_golden = (int32)output_ref[ss];
       	
       	data_golden_fp = *reinterpret_cast<float*>(&data_golden);
       	data_aie_fp = *reinterpret_cast<float*>(&data_aie);
       	     	
	diff = abs(data_golden_fp-data_aie_fp);
        if(diff > slack){

            mismatch_count++;           
        }
	
        if(diff > max_diff){
        
            max_diff = diff;
        }
        
        ss++;
    }
    
    
    if(mismatch_count != 0){
    
    	std::cout << std::endl << "MISMATCH!" << std::endl;
    	std::cout << std::endl << "Maximum error is " << max_diff << std::endl;
        std::cout << std::endl << mismatch_count << "/" << NUM_SAMPLES_O << " different samples" << std::endl;
    
    }else{
    
    	std::cout << std::endl << "MATCH!" << std::endl;
    	std::cout << std::endl << "Maximum error is " << max_diff << std::endl;
    	std::cout << std::endl << "Accepted slack is set to: " << slack << std::endl;
    
    }
    

   // output file is printed only in the case of software simulation, useful for matlab debug
   #if defined(__AIESIM__) || defined(__X86SIM__) || defined(__ADF_FRONTEND__)
    
    std::ofstream outputFile("output.txt"); 
    for (unsigned ss=0; ss < DDR_BUFFSIZE_O_BYTES/sizeof(int32); ss++) {

        uint32_t data;
        uint32_t data1; 

        data = out_bo_mapped[ss];
        data1 = out_bo_mapped[ss+1];
        outputFile << data << " " << data1 << endl;
        ss++;
    }
    outputFile.close();
    
    #endif

    return 0;
	
}

