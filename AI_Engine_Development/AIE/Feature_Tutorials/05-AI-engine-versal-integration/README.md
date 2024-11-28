﻿<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# AI Engine Versal Integration

***Version: Vitis 2024.2***

## Introduction

Versal™ adaptive SoCs combine programmable logic (PL), processing system (PS), and AI Engines with leading-edge memory and interfacing technologies to deliver powerful heterogeneous acceleration for any application. The hardware and software are targeted for programming and optimization by data scientists and software and hardware developers. A host of tools, software, libraries, IP, middleware, and frameworks enable Versal adaptive SoCs to support all industry-standard design flows.

This tutorial demonstrates creating a system design running on the AI Engine, PS, and Programmable Logic (PL). The AI Engine domain contains a simple graph consisting of three kernels. These kernels are connected by both windows and streams. The PL domain contains data movers that provide input and capture output from the AI Engine. The PS domain contains a host application that controls the entire system. You will validate the design running on these heterogeneous domains by first emulating the hardware and then running on actual hardware.

This tutorial steps through software emulation, hardware emulation, and hardware flow in the context of a complete Versal adaptive SoC system integration. By default, the Makefile is set for `hw_emu`. If you need to build for `hw`, use the corresponding TARGET option as described in corresponding sections.

**IMPORTANT**: Before beginning the tutorial ensure you have installed Vitis&trade; 2024.2 software. The software includes all the embedded base platforms including the VCK190 base platform that is used in this tutorial. In addition, ensure you have downloaded the Common Images for Embedded Vitis Platforms from this link.

https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms/2024.2.html

The 'common image' package contains a prebuilt Linux kernel and root file system that can be used with the Versal board for embedded design development using Vitis.
Before starting this tutorial run the following steps:

1. Navigate to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ```/Common Images Dir/xilinx-versal-common-v2024.2/environment-setup-cortexa72-cortexa53-xilinx-linux``` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ```/Common Images Dir/xilinx-versal-common-v2024.2/sdk.sh```.
3. Set up your ROOTFS, and IMAGE to point to the ```rootfs.ext4``` and Image files located in the ```/Common Images Dir/xilinx-versal-common-v2024.2``` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ```$XILINX_VITIS/base_platforms/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm```.

This tutorial targets VCK190 production board for 2024.2 version.

## Objectives

After completing this tutorial, you should be able to:

* Compile HLS functions for integration in the PL.
* Compile ADF graphs.
* Explore Vitis Analyzer for viewing the compilation and simulation summary reports.
* Create a configuration file that describes system connections and use it during the link stage.
* Package the design to run on software emulation, hardware emulation, and an easy-to-boot SD card image to run on hardware.
* Become familiar with the new Vitis unified IDE

This tutorial contains two flows that you can work through: the Vitis classic command line flow incorporating the aiecompiler, aiesimulator, and v++ command, and the new Vitis unified IDE flow which demonstrates the use of the new tool available in the 2024.2 release as described in the *Vitis Unified IDE and Common Command-Line Reference Guide* ([UG1393](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/Using-the-Vitis-Unified-IDE)). The classic command-line flow is documented in the following text. The new Vitis unified IDE flow can be found [here](./unified-ide.md).

## Tutorial Overview

**Section 1**: Compile AI Engine code for `aiesimulator`, viewing compilation results in Vitis Analyzer.

**Section 2**: Simulate the AI Engine graph using the `aiesimulator` and viewing trace and profile results in Vitis Analyzer.

**Section 3**: Run the hardware emulation, and view run summary in Vitis Analyzer.

**Section 4**: Run on hardware.

The design that will be used is shown in the following figure.

![System Diagram](./images/lab8_system_diagram.png)

|Kernel|Type|Comment|
|  ---  |  ---  |  ---  |
|MM2S|HLS|Memory Map to Stream HLS kernel to feed input data from DDR to AI Engine interpolator kernel via the PL DMA.|
|Interpolator |AI Engine| Half-band 2x up-sampling FIR filter with 16 coefficients. Its input and output are cint16 window interfaces and the input interface has a 16 sample margin. |
|Polar_clip|AI Engine| Determines the magnitude of the complex input vector and clips the output magnitude if it is greater than a threshold. The polar_clip has a single input stream of complex 16-bit samples, and a single output stream whose underlying samples are also complex 16-bit elements.|
|Classifier |AI Engine| This kernel determines the quadrant of the complex input vector and outputs a single real value depending which quadrant. The input interface is a cint16 stream and the output is a int32 window.  |
|S2MM|HLS|Stream to Memory Map HLS kernel to feed output result data from AI Engine classifier kernel to DDR via the PL DMA.|


## Section 1: Compile AI Engine Code for AIE Simulator: Viewing Compilation Results in Vitis Analyzer

1. Set the `SYSROOT` and `CXX` variables as mentioned in the **Introduction**.
2. Clean the `Working` directory to remove all the files that are related to the software emulation by running the following command:

    ```bash
    make clean
    ```

### Compiling an AI Engine ADF Graph for V++ Flow

To compile the graph type to be used in either HW or HW_EMU, use:

```bash
make aie TARGET=hw
```

Or

```bash
v++ -c --mode aie --target hw --platform $PLATFORM_REPO_PATHS/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm --include "$XILINX_VITIS/aietools/include" --include "./aie" --include "./data" --include "./aie/kernels" --include "./" --aie.xlopt=0 --work_dir=./Work aie/graph.cpp
```

The generated output from `aiecompiler` is the `Work` directory, and the `libadf.a` file. This file contains the compiled AI Engine configuration, graph, and Kernel `.elf` files.

#### Vitis Analyzer Compile Summary

Vitis Analyzer is used to view the AI Engine compilation results. It highlights the state of compilation, displays the graph solution in both the **Graph** and **Array** views, provides guidance around the kernel code, and allows you to open various reports produced by `aiecompiler`. Below is the `graph.aiecompile_summary` file generated by the `aiecompiler`, which is located in the `Work` directory.

To open the summary file, use the following command:

`vitis_analyzer -a ./Work/graph.aiecompile_summary`

The **Summary** View displays the compilation runtime, the version of the compiler used, the platform targeted, kernels created, and the exact command line used for the compilation.

![Vitis Analyzer Compile Summary](./images/vitis_analyzer_compile_summary.png)

1. Click **Kernel Guidance**. This view provides a list of messages (INFO, Warning, Critical Warning) and provides optimization and best practice guidance for kernel development. By default, ***INFO*** messages are hidden.

    ![Vitis Analyzer Kernel Guidance](./images/vitis_analyzer_kernel_guidance.png)

2. Click **Mapping Analysis**. This report provides detailed mapping information which the `aiecompiler` generates for mapping the graph to the AI Engine.

    ![Vitis Analyzer Mapping Analysis](./images/vitis_analyzer_mapping_analysis.png)

3. Click **DMA Analysis**. This is a text report showing a summary of the DMA accesses from the graph.

    ![Vitis Analyzer DMA Analysis](./images/vitis_analyzer_dma_analysis.png)

4. Click **Lock Allocation**. This shows the locks per buffer and where it is mapped in the ADF Graph.

    ![Vitis Analyzer Lock Allocation](./images/vitis_analyzer_lock_allocation.png)

5. Click **Log**. This is the compilation log for your graph.

    ![Vitis Analyzer Log](./images/vitis_analyzer_log.png)

6. Click **AI Engine Compilation**. This view shows the individual logs and command line options for the individual Tile compilations. The following figure shows an example of the kernel in ***Tile [24,0]***.

    ![Vitis Analyzer Core Compilation](./images/vitis_analyzer_core_compilation1.png)
    ![Vitis Analyzer Core Compilation Log](./images/vitis_analyzer_core_compilation2.png)

**Note**: The **Graph View** and **Array View** are presented in the next section.

## Section 2: Simulate the AI Engine Graph using the `aiesimulator` and Viewing Trace and Profile Results in Vitis Analyzer

After the graph has been compiled, you can simulate your design with the `aiesimulator` command. This uses a cycle-approximate model to test your graph and get preliminary throughput information early in the design cycle, while the PL developers continue to work on the platform for the application.

**Note**: Simulating the design with VCD will increase simulation runtime. To learn more about this feature, see [AI Engine SystemC Simulator](https://docs.amd.com/access/sources/dita/map?isLatest=true&ft:locale=en-US&url=ug1076-ai-engine-environment).

1. To run simulation use the command:

    ```bash
    make sim TARGET=hw
    ```

    or

    ```bash
    aiesimulator --profile --dump-vcd=tutorial --pkg-dir=./Work
    ```

    | Flag | Description |
    | ---- | ----------- |
    | --profile | Profiles all kernels, or select kernels (col,row)...(col,row).|
    | --dump-vcd | Grabs internal signals of tiles and dumps it in a VCD file. |
    | --pkg-dir | The ***Work*** directory. |

2. When simulation has completed, use a terminal to navigate to the `aiesimulator_output` directory by running: `cd aiesimulator_output; ls`

   You should see something similar to this:

    ```bash
    aiesim_options.txt      profile_funct_24_0.xml  profile_funct_25_0.xml  profile_instr_24_0.xml  profile_instr_25_0.xml
    data                    profile_funct_24_1.txt  profile_funct_25_1.txt  profile_instr_24_1.txt  profile_instr_25_1.txt  
    default.aierun_summary  profile_funct_24_1.xml  profile_funct_25_1.xml  profile_instr_24_1.xml  profile_instr_25_1.xml
    profile_funct_24_0.txt  profile_funct_25_0.txt  profile_instr_24_0.txt  profile_instr_25_0.txt
    ```

    The files prefixed with `profile_` are the outputs of the profiling and calculated per tile. In this tutorial, profiling is done for all tiles that are used, but you can limit profiling to specific tiles by providing the row and column of the tile. For more information about profiling with `aiesimulator` see [here](https://docs.amd.com/r/en-US/ug1076-ai-engine-environment/Simulating-an-AI-Engine-Graph-Application). You can open up these files to see what was calculated, but it is better to view it in Vitis Analyzer where it is curated. The `data` directory is generated here with the output file(s) you have in the `graph.cpp` for the PLIO objects. Finally, the `default.aierun_summary` is generated, which contains all the information generated by `aiesimulator` with profiling and trace information. Opening this file in Vitis Analyzer allows you to browse all the output files, and profile/trace data.

    **NOTE**: The `tutorial.vcd` is generated on the same level as the `./Work` directory.

    You can now open the generated `default.aierun_summary` from the `aiesimulator_output` directory for Vitis Analyzer.

3. To do this,run the command:

    ```bash
    vitis_analyzer -a ./aiesimulator_output/default.aierun_summary
    ```

    With this tool you can use a variety of views to debug and potentially optimize your graph.

    The **Summary** view provides an overview of running `aiesimulator`. As you can see in the following figure, it provides information on status, version used, time, platform used, and the command line used to execute.

    ![Vitis Analyzer Summary](./images/vitis_analyzer_summary.png)

4. Click **Profile**.

    The **Profile View** provides detailed information collected during the simulation. Information includes cycle count, total instructions executed, program memory, and specific information per functions in the two tiles that the kernels are programmed.

    ![Vitis Analyzer Profile](./images/vitis_analyzer_profile.png)

    This is the top-level view of the profile. The left column allows you to select one of many types of reports generated per function.

5. Select the first **Total Function Time** from this column. You will see the following.

    ![Vitis Analyzer Profile 1 Kernel](./images/vitis_analyzer_profile_one_kernel.png)

    In this chart you can see what function is called most, function time, etc. This information can be useful in determining if the tile is under- or over-utilized in your design.

6. Click **Graph**.

    The **Graph** view provides an overview of your graph and how the graph is designed in a logical fashion. In this view, you can see all the PLIO ports, kernels, buffers, and net connections for the entire ADF Graph.

    **Note**: This view, as well as the **Array** view have cross-probe selection, meaning selecting an object in this view will select it in the other and vice versa.

    ![Vitis Analyzer Graph](./images/vitis_analyzer_graph.png)

7. Click **Array**.

    The **Array** view provides a logical device view of the AI Engine, kernel placement, and how they are connected to each other as well as the shim.

    ![Vitis Analyzer Array](./images/vitis_analyzer_array.png)

    * Cross probe to kernel and graph source files.
    * The table at the bottom shows the following:
      * **Kernel** - The kernels in the graph.
      * **PL** - Shows connections between the graph and PLIO.
      * **Buffer** - Shows all the buffers used for inputs/outputs of the graph and the buffers for kernels.
      * **Port** - Shows all the ports of each kernel and ADF Graph.
      * **Net** - Shows all nets, named and generated, mapped in the ADF Graph.
      * **Tile** - Shows tile data (kernels, buffers) of mapped tiles and their grid location.

    **Tip**: For more detailed information about these tables, see [Section: "Viewing Compilation Results in the Vitis Analyzer"](https://docs.amd.com/r/en-US/ug1076-ai-engine-environment/Viewing-Compilation-Results-in-the-Analysis-View-of-the-Vitis-Unified-IDE).

    You can zoom into the view to get finer detail of the AI Engine and see how tiles are made up as seen in the following screenshot.

8. To zoom in, click and drag from the upper-left to the lower-right of the area you want to view to have a box show up around the area to zoom. Below is a zoomed-in area.

    ![Vitis Analyzer Array Zoom-In](./images/vitis_analyzer_array_zoom.png)

    In this zoomed in location you can see how the kernels are connected to a variety of tiles and how the shim is connected to the PLIO ports of this design.

9. Click **Simulator Output**.

    Finally, the **Simulator Output** view. This will print out the `output.txt` generated by the graph. This is a timestamped output.

    **Note**: If you need to compare this file to a golden one, you will need have to remove the -`T ####ns`- from the file.

    ![Vitis Analyzer Output](./images/vitis_analyzer_output.png)

    If you need to make any changes to the ADF Graph or the kernels inside based on results of the `aiesimulator` you can do so and re-run the compiler and view the results in Vitis Analyzer to see the changes you have made.

10. When you are done with Vitis Analyzer, close it by clicking **File** > **Exit**.

## Section 3: Run the Hardware Emulation, and View Run Summary in Vitis Analyzer

### 1. Compiling HLS Kernels Using v++

Compile the `mm2s`, and `s2mm` PL HLS kernels using the `v++` compiler command - which takes in an HLS kernel source and produces an `.xo` file.

To compile the kernels, run the following command:

```bash
make kernels TARGET=hw_emu
```

or

```bash
v++ -c --mode hls --platform $PLATFORM_REPO_PATHS/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm --config pl_kernels/s2mm.cfg
v++ -c --mode hls --platform $PLATFORM_REPO_PATHS/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm --config pl_kernels/mm2s.cfg
```

### 2. Use V++ to Link AI Engine, HLS Kernels with the Platform

After the AI Engine kernels, graph, PL kernel, and HLS kernels have been compiled and simulated, you can use `v++` to link them with the platform to generate an `.xsa`.

Use the `system.cfg` configuration file to connect the AI Engine and PL kernels in the design.

```ini
[connectivity]
nk=mm2s:1:mm2s
nk=s2mm:1:s2mm
sc=mm2s.s:ai_engine_0.DataIn1
sc=ai_engine_0.DataOut1:s2mm.s
```

To build the design, run the following command:

```bash
make xsa TARGET=hw_emu
```

or

```bash
v++ -l --platform $PLATFORM_REPO_PATHS/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm s2mm.xo mm2s.xo libadf.a -t hw_emu --save-temps -g --config system.cfg -o tutorial.xsa
```

Now you have a generated `.xsa` that will be used to execute your design on the platform.

### 3.Compile the A72 Host Application

To compile the A72 host application, run the command:

```bash
make host TARGET=hw_emu
```

Or

```bash
cd ./sw

aarch64-xilinx-linux-g++ -Wall -c -std=c++14 -Wno-int-to-pointer-cast --sysroot=$SDKTARGETSYSROOT -I$SDKTARGETSYSROOT/usr/include/xrt -I$SDKTARGETSYSROOT/usr/include -I./ -I../aie -I$XILINX_VITIS/aietools/include -I$XILINX_VITIS/include -o main.o .cpp

aarch64-xilinx-linux-g++ main.o -lxrt_coreutil -L$SDKTARGETSYSROOT/usr/lib --sysroot=$SDKTARGETSYSROOT -L$XILINX_VITIS/aietools/lib/aarch64.o -o host.exe
cd ..
```

### 4.Package the Design

With all the AI Engine outputs and the new platform created, you can now generate the Programmable Device Image (PDI) and a package to be used on an SD card. The PDI contains all executables, bitstreams, and configurations of every element of the device. The packaged SD card directory contains everything to boot Linux and have your generated application, and `.xclbin`.

To package the design, run the following command:

```bash
make package TARGET=hw_emu
```

Or

```bash
cd ./sw
v++ --package -t hw_emu \
    -f $PLATFORM_REPO_PATHS/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm \
    --package.rootfs=$PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2024.2/rootfs.ext4 \
    --package.image_format=ext4 \
    --package.boot_mode=sd \
    --package.kernel_image=$PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2024.2/Image \
    --package.defer_aie_run \
    --package.sd_file host.exe ../tutorial.xsa ../libadf.a
cd ..
```

**NOTE:** By default the `--package` flow will create a `a.xclbin` automatically if the `-o` switch is not set.


### 5.Run Hardware Emulation

After packaging, everything is set to run emulation. Since you ran `aiesimulator` with profiling enabled, you can bring that to hardware emulation. You can pass the `aiesim_options.txt` to the `launch_hw_emu.sh` which will enable the profiling options used in `aiesimulator` to be applied to hardware emulation. To do this, add the `-aie-sim-options ../aiesimulator_output/aiesim_options.txt`.
Since Profiling is deprecated in Hardware Emulation Flow, comment the line 'AIE_PROFILE=All' in `aiesimulator_output/aiesim_options.txt`

1. To run emulation use the following command:

    ```bash
    make run_emu TARGET=hw_emu
    ```

    or

    ```bash
    cd ./sw
    ./launch_hw_emu.sh -aie-sim-options ../aiesimulator_output/aiesim_options.txt -add-env AIE_COMPILER_WORKDIR=../Work
    ```

    When launched, use the Linux prompt presented to run the design.  Note that the emulation process is slow, so do not touch the keyboard of your terminal or you might stop the emulation of the Versal booth (as it happens in the real HW board).

2. Execute the following command when the emulated Linux prompt appears:

    ```bash
    cd /run/media/*1
    export XILINX_XRT=/usr
    dmesg -n 4 && echo "Hide DRM messages..."
    ```

    This will set up the design to run emulation and remove any unnecessary DRM messaging.

3. Run the design using the following command:

    ```bash
    ./host.exe a.xclbin
    ```

    **Note**: The design runs with dumping VCD, which will extend emulation time. It may seem as if it is hung, but it is not.

4. You should see an output displaying **TEST PASSED**. When this is shown, run the keyboard command: `Ctrl+A x` to end the QEMU instance.

5. To view the profiling results and trace in Vitis Analyzer, run the command:

    ```bash
    vitis_analyzer -a sw/sim/behav_waveform/xsim/default.aierun_summary
    ```

    ![hw_emu analyzer](./images/hw_emu_analyzer.png)

    When you open the run Summary, you will notice that it is the same layout as that from `aiesimulator`.

6. Click **Trace**. This will open up the VCD data (as defined in the `aiesim_options.txt`). This gives detailed information about kernels, tiles, and nets within the AI Engine during execution. Here you can see stalls in regards to each kernel and can help you identify where they are originating.

    ![hw_emu trace](./images/hw_emu_trace.png)

7. From the trace information, you can calculate the kernel latency as follows:

   Click the Trace in the AI Engine simulation run summary, and navigate to the any function to calculate the latency. For example, consider the classifier function. 
   You can notice the function classifier ran for seven iterations. Zoom into the period of one iteration (between two main() function calls as follows), add a  marker, and drag it to the end of the kernel function as follows:
    ![hw_emu_trace](./images/latency_calc.png)
    
    Notice the difference of 25.093 us as highlighted above. This is the time the kernel took to complete one iteration.

   If you click the AI Engine Simulation Summary, you can notice the AI Engine Frequency as 1250 MHz, i.e., 0.8 ns, i.e., one cycle = 0.8 ns. Now, the classifier function took 23.854 us for one iteration, i.e., 23.854 us / 0.8 ns ~= 29817 cycles. Compare this with the latency you got during the aiesimulation where the AI Engine is a standalone module;
   
   This Latency calculated can also be compared against the value(average Latency) displayed in the trace.

8. Explore the two reports and take note of any differences and similarities. This will help you debug and optimize your design.

9. Open the `aiesimulator` run Summary by clicking **File** > **Open Summary**, navigating to the `aiesimulator_output` directory, and clicking `default.aierun_summary`.

    Explore the two reports and take note of any differences and similarities. This will help you debug and optimize your design.

10. Close out of the **Vitis Analyzer** and build for hardware.

## Section 4: Build and Run on Hardware

1. To build for hardware, run the following command:

    ```bash
    cd ..
    make xsa TARGET=hw
    ```

    or

    ```bash
    v++ -l --platform $PLATFORM_REPO_PATHS/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm s2mm.xo mm2s.xo libadf.a -t hw --save-temps -g --config system.cfg -o tutorial.xsa
   ```

2. Then re-run the packaging step with:

    ```bash
    make package TARGET=hw
    ```

    or

    ```bash
    cd ./sw
    v++ --package -t hw \
        -f $PLATFORM_REPO_PATHS/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm \
        --package.rootfs=$PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2024.2/rootfs.ext4 \
        --package.image_format=ext4 \
        --package.boot_mode=sd \
        --package.kernel_image=$PLATFORM_REPO_PATHS/sw/versal/xilinx-versal-common-v2024.2/Image \
        --package.defer_aie_run \
        --package.sd_file host.exe ../tutorial.xsa ../libadf.a
    cd ..
    ```

    When you run on hardware, ensure you have a supported SD card. Format the SD card with the `sw/sd_card.img` file. Then plug the SD card into the board and power it up.

3. When a Linux prompt appears, run the following commands:

    ```bash
    dmesg -n 4 && echo "Hide DRM messages..."
    cd /run/media/*1
    export XILINX_XRT=/usr
    ./host.exe a.xclbin
    ```

You should see **TEST PASSED**. You have successfully run your design on hardware.

**IMPORTANT**: To re-run the application you need to power cycle the board.

### Summary

In this tutorial you learned the following:

* How to compile PLIO and PL Kernels using `v++ -c`.
* How to link the `libadf.a`, PLIO, and PL kernels to the `xilinx_vck190_base_202410_1` platform.
* How to use Vitis Analyzer to explore the various reports generated from compilation and emulation/simulation.
* How to package your host code, and the generated `xclbin` and `libadf.a` into an SD card directory.
* How to execute the design for hardware emulation.
* How to execute the design on the board.

To read more about the use of Vitis in the AI Engine flow see: [UG1076: AI Engine Tools and Flows User Guide: Integrating the Application Using the Vitis Tool Flow](https://docs.amd.com/access/sources/dita/map?isLatest=true&ft:locale=en-US&url=ug1076-ai-engine-environment).

#### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [support.xilinx.com](https://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
