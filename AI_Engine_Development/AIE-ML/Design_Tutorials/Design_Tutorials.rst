##############################################################################
AIE-ML Development: Design Tutorials
##############################################################################


.. sidebar:: More Information

   See AMD Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_



The AI Engine Development Design Tutorials showcase the two major phases of AI Engine-ML application development: architecting the application and developing the kernels. Both phases are demonstrated in these tutorials.


.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   AIE-ML Programming <./01-AIE-ML-programming-and-optimization/README.md>
   Prime Factor FFT-1008 on AIE-ML <./02-Prime-Factor-FFT/README.md>
   AIE-ML LeNet Tutorial <./03-AIE-ML-lenet_tutorial/README.md>
   AIE API based FFT for Many Instances Applications <./04-AIE-API-based-FFT-for-many-instances-applications/README.md>
   Softmax Function on AIE-ML <./05-Softmax-Function/README.md>
   Migrating Farrow Filter from AIE to AIE-ML <./06-farrow_filter/README.md>
   Polyphase Channelizer on AIE-ML using Vitis Libraries <./07-Channelizer-Using-Vitis-Libraries/README.md>
   MNIST ConvNet on AIE-ML <./08-MNIST-ConvNet/README.md>








.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description
	 

   * - :doc:`AIE-ML Programming <./01-AIE-ML-programming-and-optimization/README>`
     - This tutorial helps user understand the differences between AI Engine and AI Engine-ML architecture, it is based on matrix multiplication which is a usual algorithm in Machine Learning applications.

   * - :doc:`Prime Factor FFT-1008 on AIE-ML <./02-Prime-Factor-FFT/README>`
     - This Versal system example implements a 1008-pt FFT using the Prime Factor algorithm. The design uses both AI Engine and PL kernels working cooperatively. AI Engine elements are hand-coded using AIE API, PL elements are implemented using Vitis HLS. System integration in Vitis is managed using the new v++ Unified Command Line flow. This tutorial targets the AIE-ML architecture.

   * - :doc:`AIE-ML LeNet Tutorial <./03-AIE-ML-lenet_tutorial/README>`
     - This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AIE-ML architecture and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AIE-ML and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM), Memory tile and AI Engine memory.

   * - :doc:`AIE API based FFT for Many Instances Applications <./04-AIE-API-based-FFT-for-many-instances-applications/README>`
     - This tutorial walks the user through the design and the implementation of an FFT for many parallel signals on a Real-Time system, using the AI Engine APIs. The design performance objective is minimizing power and utilization, maintaining a high throughput to at least match the Real-Time acquisition bandwidth. Moreover, the design leverages the AIE-ML Memory Tiles to minimize programmable logic utilization. The considered case study comprises 128 parallel signals, each with a 125MSa/s sample rate and CINT16 datatype, with a total aggregated bandwidth of 64GBytes/s.

   * - :doc:`Softmax Function on AIE-ML <./05-Softmax-Function/README>`
     - This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AIE-ML architecture and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AIE-ML and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM), Memory tile and AI Engine memory.

   * - :doc:`Migrating Farrow Filter from AIE to AIE-ML <./06-farrow_filter/README>`
     - This tutorial uses the LeNet algorithm to implement a system-level design to perform image classification using the AIE-ML architecture and PL logic, including block RAM (BRAM). The design demonstrates functional partitioning between the AIE-ML and PL. It also highlights memory partitioning and hierarchy among DDR memory, PL (BRAM), Memory tile and AI Engine memory.

   * - :doc:`Polyphase Channelizer on AIE-ML using Vitis Libraries <./07-Channelizer-Using-Vitis-Libraries/README.md>`
     - This tutorial demonstrates how to leverage Vitis Libraries IP blocks to build a high performance Polyphase Channelizer on AIE-ML running at 2 GSPS.

   * - :doc:`MNIST ConvNet on AIE-ML <./08-MNIST-ConvNet/README.md>`
     - This tutorial implements a Convolutional Neural Network classifier on AIE-ML for identifying hand-written digits from the MNIST database. The goal is to illustrate how to partition & vectorize a simple machine learning example to Versal AI Engines.














