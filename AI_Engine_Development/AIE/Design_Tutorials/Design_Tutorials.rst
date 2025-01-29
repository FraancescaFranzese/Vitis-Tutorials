##############################################################################
AI Engine Development: Design Tutorials
##############################################################################


.. sidebar:: More Information

   See Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_



The AI Engine Development Design Tutorials showcase the two major phases of AI Engine application development: architecting the application and developing the kernels. Both phases are demonstrated in these tutorials.

The :doc:`landing page <../AIE>` of AI Engine Development contains important information including tool version, environment settings, and a table describing the platform, operating system, and supported features or flows of each tutorial. It is strongly recommended that you review details before starting to use the AIE tutorials.




.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Versal Custom Thin Platform Extensible System <./01-Versal_Custom_Thin_Platform_Extensible_System/README.md>
   LeNet Tutorial <./01-aie_lenet_tutorial/README.md>
   Super Sampling Rate FIR Filters <./02-super_sampling_rate_fir/README.md>
   Beamforming Design <./03-beamforming/README.md>
   Polyphase Channelizer <./04-Polyphase-Channelizer/README.md>
   Prime Factor FFT <./05-Prime-Factor-FFT/README.md>
   2D-FFT <./06-fft2d_AIEvsHLS/README.md>
   FIR Filter <./07-firFilter_AIEvsHLS/README.md>
   N-Body Simulator <./08-n-body-simulator/README.md>
   Digital Down-conversion Chain: Converting from Intrinsics to API <./09-ddc_chain/README.md>
   Versal GeMM Implementation <./10-GeMM_AIEvsDSP/README.md>
   Bilinear Interpolation <./11-Bilinear_Interpolation/README.md>
   64K IFFT Using 2D Architecture <./12-IFFT64K-2D/README.md>
   Implementing FFT and DFT Designs on AI Engines <./13-FFT-DFT-on-AIE/README.md>
   Bitonic SIMD Sorting on AI Engine for float Datatypes <./14-Bitonic-Sorting/README.md>
   Fractional Delay Farrow Filter <./15-farrow_filter/README.md>
   1 Million Point float FFT @ 32 Gsps on AI Engine <./16-1M-Point-FFT-32Gsps/README.md>
   System Partitioning of a Hough Transform on AI Engine <./17-Hough-Transform/README.md>
   MUSIC Algorithm <./18-MUSIC-Algorithm/README.md>
   Softmax Function <./19-Softmax-Function/README.md>





.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 
   * - :doc:`Versal Custom Thin Platform Extensible System <./01-Versal_Custom_Thin_Platform_Extensible_System/README>`
     - This is a Versal system example design based on a VCK190 thin custom platform (Minimal clocks and AXI exposed to PL) that includes HLS/RTL kernels and AI Engine kernel using a full Makefile build-flow.

   * - :doc:`LeNet Tutorial <./01-aie_lenet_tutorial/README>`
     - This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AI Engine and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AI Engine and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM) and AI Engine memory.

   * - :doc:`Super Sampling Rate FIR Filters <./02-super_sampling_rate_fir/README>`
     - The purpose of this tutorial is to provide a methodology to enable you to make appropriate choices depending on the filter characteristics, and to provide examples on how to implement Super Sampling Rate (SSR) FIR Filters on a Versal® ACAP AI Engine processor array.

   * - :doc:`Beamforming Design <./03-beamforming/README>`
     - This tutorial demonstrates the creation of a beamforming system running on the AI Engine, PL, and PS, and the validation of the design running on this heterogeneous domain.

   * - :doc:`Polyphase Channelizer <./04-Polyphase-Channelizer/README>`
     - This tutorial demonstrates an implementation of a system-level design (such as Polyphase Channelizer) using a combination of AI Engine and PL/HLS kernels.

   * - :doc:`Prime Factor FFT-1008 <./05-Prime-Factor-FFT/README>`
     - This Versal system example implements a 1008-pt FFT using the Prime Factor Algorithm. The design uses both AI Engine and PL kernels working cooperatively. AI Engine elements are hand-coded using AIE API. PL elements are implemented using Vitis HLS. System integration in Vitis is managed using the new v++ Unified Command Line flow.

   * - :doc:`2D-FFT <./06-fft2d_AIEvsHLS/README>`
     - This tutorial performs two implementations of a system-level design (2D-FFT): one with AI Engine, and the other with HLS using the DSP Engines.

   * - :doc:`FIR Filter <./07-firFilter_AIEvsHLS/README>`
     - This tutorial demonstrates the implementations of a system-level design (FIR Filter) using AI Engines and HLS with DSP Engines in the Versal device plus PL including LUTs, flip-flops (FFs), and block RAMs.

   * - :doc:`N-Body Simulator <./08-n-body-simulator/README>`
     - It is a system-level design that uses the AI Engine, PL, and PS resources to showcase the following features:

       - A Python model of an N-Body Simulator run on x86 machine
       - A scalable AI Engine design that can utilize up to 400 AI Engine tiles
       - AI Engine packet switching
       - AI Engine single-precision floating point calculations
       - AI Engine 1:400 broadcast streams
       - Codeless PL HLS datamover kernels from the Vitis™ Utility Library
       - PL HLS packet switching kernels
       - PS Host Application that validates the data coming out of the AI Engine design
       - C++ model of an N-Body Simulator
       - Performance comparisons between Python x86, C++ Arm A72, and AI Engine N-Body Simulators
       - Effective throughput calculation (GFLOPS) vs. Theoretical peak throughput of AI Engine

   * - :doc:`Digital Down-conversion Chain - Converting from Intrinsics to API <./09-ddc_chain/README>`
     - This tutorial demonstrates the steps to upgrade a 32-branch digital down-conversion chain so that it is compliant with the latest tools and coding practice.

   * - :doc:`Versal GeMM Implementation <./10-GeMM_AIEvsDSP/README>`
     - This tutorial performs two implementations of a system-level design: one with AI Engine, and the other with RTL using the DSP Engines. In each implementation, the tutorial takes you through the hardware emulation and hardware flow in the context of a complete Versal ACAP system design.

   * - :doc:`Bilinear Interpolation <./11-Bilinear_Interpolation/README>`
     - This tutorial demonstrates how the bilinear interpolation algorithm may be efficiently implemented using AI Engines. It also provides guidance for customizing the design to function with varying image resolutions, and to take advantage of multicore processing on the AI Engine array to achieve desired throughput.

   * - :doc:`64K IFFT Using 2D Architecture <./12-IFFT64K-2D/README>`
     - This Versal system example implements a 64K-pt IFFT using a 2D architecture. We decompose ``64K = 256 x 256`` and build the transform in two dimensions using row and column FFT-256. A matrix transpose is performed in between in the PL. This alternative "divide and conquer" approach is attractive in the ``SSR > 1`` regime.

   * - :doc:`Implementing FFT and DFT Designs on AI Engines <./13-FFT-DFT-on-AIE/README>`
     - This tutorial illustrates several techniques for mapping FFT and DFT algorithms to the AI Engine array including the Stockham FFT used in AMD Vitis DSPlib, hand-coded variants implemented using the AI Engine API, and a direct form DFT using vector-matrix multiplication. We also illustrate how to trade off AI engine tile resource vs. throughput performance of the Stockham FFT in DSPlib using its ``TP_CASC_LEN`` and ``TP_PARALLEL_POWER`` template parameters. This is useful when configuring DSPlib FFT library instances to serve as part of a larger 2D FFT architecture.

   * - :doc:`Bitonic SIMD Sorting on AI Engine for float Datatypes <./14-Bitonic-Sorting/README>`
     - This tutorial illustrates how to implement a Bitonic SIMD sorter on AI Engine in Versal for ``float`` data types. Two examples are given. First, a small example using ``N=16`` demonstrates the concept and identifies strategies for vectorization & management of the vector register space. These ideas are then applied to a second larger example using ``N=1024``. Profiling & throughput performance are compared to ``std::sort()``.

   * - :doc:`Fractional Delay Farrow Filter <./15-farrow_filter/README>`
     - This Versal system example implements a variable fractional delay algorithm using the Farrow Filter structure and walks the user through common AI Engine design optimization techniques. The design uses both AI Engine and PL kernels working cooperatively. AI Engine elements are hand-coded using AIE API. PL elements are implemented using Vitis HLS. System integration in Vitis is managed using the new v++ Unified Command Line flow.

   * - :doc:`1 Million Point float FFT @ 32 Gsps on AI Engine <./16-1M-Point-FFT-32Gsps/README>`
     - This tutorial implements a 1M-point FFT for cfloat data types that achieves an impressive throughput rate exceeding 32 Gsps using a large portion of the AI Engine array for compute and PL URAM resources to implement a matrix transpose operation.

   * - :doc:`System Partitioning of a Hough Transform on AI Engine <./17-Hough-Transform/README>`
     - This tutorial walks through the process of planning the implementation of a well-known image processing algorithm, mapping and partitioning it to the resources available in a Versal Adaptive SoC device. We illustrate this using the Hough Transform—a feature extraction technique for computer vision and image processing.

   * - :doc:`MUSIC Algorithm <./18-MUSIC-Algorithm/README>`
     - This tutorial implements the Multiple Signal Classification (MUSIC) Algorithm on the AI Engine. MUSIC is a popular algorithm for Direction of Arrival (DOA) estimation in antenna array systems.

   * - :doc:`Softmax Function <./19-Softmax-Function/README>`
     - Softmax is an activation function often used in the output layer of a neural network designed for multi-class classification. This tutorial provides an example of how the softmax function might be implemented for those desiring to develop custom machine learning inference applications on AI Engines. 



