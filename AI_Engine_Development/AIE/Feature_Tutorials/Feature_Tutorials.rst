##############################################################################
AI Engine Development: Feature Tutorials
##############################################################################


.. sidebar:: More Information

   See Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_



The AI Engine Development Feature Tutorials highlight specific features and flows that help develop AI Engine applications.

The :doc:`landing page <../AIE>` of AI Engine Development contains important information including tool version, environment settings, and a table describing the platform, operating system, and supported features or flows of each tutorial. It is strongly recommended that you review details before starting to use the AIE tutorials.




.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   AI Engine A-to-Z Flow for Linux <./18-aie_a_to_z_custom_linux_platform/README.md>
   A to Z Bare-metal Flow <./01-aie_a_to_z/README.md>
   Using GMIO with AIE <./02-using-gmio/README.md>
   Runtime Parameter Reconfiguration <./03-rtp-reconfiguration/README.md>
   Packet Switching <./04-packet-switching/README.md>
   Versal Integration for Hardware Emulation and Hardware <./05-AI-engine-versal-integration/README.md>
   Versal System Design Clocking <./06-versal-system-design-clocking-tutorial/README.md>
   Using Floating-Point in the AI Engine <./07-AI-Engine-Floating-Point/README.md>
   DSP Library Tutorial <./08-dsp-library/README.md>
   Debug Walkthrough <./09-debug-walkthrough/README.md>
   AI Engine DSP Library and Model Composer Tutorial <./10-aie-dsp-lib-model-composer/README.md>
   Versal Emulation Waveform Analysis <./11-ai-engine-emulation-waveform-analysis/README.md>
   AI Engine Performance and Deadlock Analysis Tutorial <./13-aie-performance-analysis/README.md>
   Implementing an IIR Filter on the AI Engine <./14-implementing-iir-filter/README.md>
   Post-Link Recompile of an AI Engine Application <./15-post-link-recompile/README.md>
   Python and C++ External Traffic Generators for AI Engine Simulation and Emulation Flows <./16-external-traffic-generator-aie/README.md>
   Using RTL IP with AI Engines <./17-RTL-IP-with-AIE-Engines/README.md>
   AIE Compiler Features <./20-aiecompiler-features/README.md>
   Two Tone Filter on AIE Using DSP libraries and Vitis Model Composer <./21-two_tone_filter/README.md>
   AIE PL Interface <./24-aie_pl_interface/README.md>













.. list-table:: 
   :widths: 20 80
   :header-rows: 1
   
   * - Tutorial
     - Description

   * - :doc:`AI Engine A-to-Z Flow for Linux <./18-aie_a_to_z_custom_linux_platform/README>`
     - Intoduces a platform-based approach to develop the adaptable subsystem that contains PL kernels and AI Engine graph, demonstrates how you can quickly validate the design by means of hardware emulation or hardware using the base platform, and quickly switch to the custom platform with minimal changes.

   * - :doc:`A to Z Bare-metal Flow <./01-aie_a_to_z/README>`
     - This tutorial walks through the steps to create a custom Baremetal platform, and also integrate Baremetal host application along with  an AI Engines graph and PL kernels.

   * - :doc:`Using GMIO with AIE <./02-using-gmio/README>`
     - Introduces the usage of global memory I/O (GMIO) for sharing data between the AI Engines and external DDR.

   * - :doc:`Runtime Parameter Reconfiguration <./03-rtp-reconfiguration/README>`
     - Learn how to dynamically update AI Engine runtime parameters.

   * - :doc:`Packet Switching <./04-packet-switching/README>`
     - This tutorial illustrates how to use data packet switching with AI Engine designs to optimize efficiency.

   * - :doc:`AI Engine Versal Integration for Hardware Emulation and Hardware <./05-AI-engine-versal-integration/README>`
     - This tutorial demonstrates creating a system design running on the AI Engine, PS, and PL and validating the design running on these heterogeneous domains by running Hardware Emulation.

   * - :doc:`Versal System Design Clocking <./06-versal-system-design-clocking-tutorial/README>`
     - This tutorial demonstrates clocking concepts for the Vitis compiler by defining clocking for ADF graph PL kernels and PLIO kernels, using the clocking automation functionality.

   * - :doc:`Using Floating-Point in the AI Engine <./07-AI-Engine-Floating-Point/README>`
     - These examples demonstrate floating-point vector computations in the AI Engine.

   * - :doc:`DSP Library Tutorial <./08-dsp-library/README>`
     - This tutorial demonstrates how to use kernels provided by the DSP library for a filtering application, how to analyze the design results, and how to use filter parameters to optimize the design's performance using simulation.

   * - :doc:`Debug Walkthrough <./09-debug-walkthrough/README>`
     - This tutorial demonstrates how to debug a multi-processor application using the Versal adaptive SoC AI Engines, using a beamformer example design. The tutorial illustrates functional debug and performance level debug techniques.

   * - :doc:`AI Engine DSP Library and Model Composer Tutorial <./10-aie-dsp-lib-model-composer/README>`
     - This tutorial shows how to design AI Engine applications using Model Composer. This set of blocksets for Simulink is used to demonstrate how easy it is to develop applications for Xilinx devices, integrating RTL/HLS blocks for the Programmable Logic, as well as AI Engine blocks for the AI Engine array.

   * - :doc:`Versal Emulation Waveform Analysis <./11-ai-engine-emulation-waveform-analysis/README>`
     - This tutorial demonstrates how you can use the Vivado logic simulator (XSIM) waveform GUI, and the Vitis analyzer to debug and analyze your design for a Versal adaptive SoC.

   * - :doc:`AI Engine Performance and Deadlock Analysis Tutorial <./13-aie-performance-analysis/README>`
     - This tutorial introduces you to performance analysis and optimization methods, and shows you how synchronization works in graph execution. It also demonstrates the analysis of a hang issue using an example.

   * - :doc:`Implementing an IIR Filter on the AI Engine <./14-implementing-iir-filter/README>`
     - This multi-part tutorial describes how to implement an `infinite impulse response (IIR) filter <https://en.wikipedia.org/wiki/Infinite_impulse_response>`_ on the AI Engine.

   * - :doc:`Post-Link Recompile of an AI Engine Application <./15-post-link-recompile/README>`
     - This tutorial shows you how to modify an AI Engine application after the platform has been frozen. It avoids a complete Vivado® tool run, which can take a long time if timing closure requires specific attention. The only limitation is that the hardware connection between the AI Engine array and the programmable logic (PL) must be kept fixed. The tutorial demonstrates a Vitis IDE flow and a Makefile flow.

   * - :doc:`Python and C++ External Traffic Generators for AI Engine Simulation and Emulation Flows <./16-external-traffic-generator-aie/README>`
     - This tutorial develops a case in which the dataflow goes back and forth multiple times between the programmable logic (PL) and the AI Engine array. Some PL blocks are only source or destination kernels, whereas others are processing kernels within the dataflow. This tutorial demonstrates how to create external traffic generators as Python scripts or C++ applications to exercise the AI Engine kernels in the x86 simulator, AI Engine simulator, and in hardware emulation.

   * - :doc:`Using RTL IP with AI Engines <./17-RTL-IP-with-AIE-Engines/README>`
     - This tutorial demonstrates how to reuse any AXI-based IP you have created as an RTL IP, control your platform, and convert your RTL IP to an RTL kernel allowing for a more streamlined process for creating the design you need.

   * - :doc:`AIE Compiler Features <./20-aiecompiler-features/README>`
     - This tutorial shares a variety of features that are useful for AI Engine and AI Engine-ML (AIE-ML) programming to create more visible and efficient code compared to early versions of the compiler.

   * - :doc:`Two Tone Filter on AIE Using DSP libraries and Vitis Model Composer <./21-two_tone_filter/README>`
     - This tutorial demonstrates how to implement the same MATLAB model design using the Vitis DSP libraries targeting AI Engine. This MATLAB model design, which has a two tone input signal. The FIR suppresses 1-tone from a 2-tone input signal. The output of the FIR filter connects to the FFT block. This FFT block acts as a monitor to display a spectrum plot. This tutorial has four parts. In part 1, the sampling rate requirement is 400 Msps. In part 2, the sampling rate requirement is 2000 Msps. In part 3, implement part 1 of the design using Vitis IDE. In part 4, implement part 1 of the design using Vitis Model Composer tool.

   * - :doc:`AIE PL Interface <./24-aie_pl_interface/README>`
     - This tutorial shows ways of interfacing user RTL logic to the AI Engine using the Vitis acceleration flow.









