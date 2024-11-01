﻿<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1> Vitis™ Getting Started Tutorials </h1>
   <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a> </td>
 </tr>
</table>

# Vitis Getting Started Tutorial

## Part 4: Build and Run the Data Center Application

 In this fourth part of the Introduction to Vitis tutorial, you will compile and run the vector-add example using each of three build targets supported in the Vitis flow as described below. The overall flow is described in [Introduction to Data Center Acceleration for Software Programmers](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/Introduction-to-Data-Center-Acceleration-for-Software-Programmers), and includes the image flow diagram shown below. From the image you can see the standard G++ compilation process for the host application, and the use of the Vitis compiler (`v++`) for building the AMD device binary (`xclbin`). These are the steps you will be working through in this lab. 

![img](./images/data_center_flow.png)

* Software Emulation: The kernel code is compiled to run on the host processor. This allows iterative algorithm refinement through fast build-and-run loops. This target is useful for identifying syntax errors, performing source-level debugging of the kernel code running together with application, and verifying the behavior of the system. Refer to [Software Emulation](https://docs.amd.com/access/sources/dita/topic?Doc_Version=2024.1%20English&url=ug1393-vitis-application-acceleration&resourceid=using-software-emulation-xvc1504034328357.html) for more information.

* Hardware Emulation: The kernel code is compiled into a hardware model (RTL), which is run in a dedicated simulator. This build-and-run loop takes longer but provides a detailed, cycle-accurate view of kernel activity. This target is useful for testing the functionality of the logic that will go in the FPGA and getting initial performance estimates. Refer to [Hardware Emulation](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/Hardware-Emulation) for more information.

* Hardware: The kernel code is compiled into a hardware model (RTL) and then implemented on the FPGA, resulting in a binary that will run on the actual FPGA. Refer to [System Hardware Target](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/System-Hardware-Target) for more information.

## Building and Running on Alveo U250 Data-Center Accelerator Card

### Setting Up the Environment

> **IMPORTANT:** This tutorial requires Vitis 2023.1 or later to run.

>**NOTE:** The instructions provided below assume that you are running in a bash shell.

* To configure the environment to run Vitis, source the following scripts:

  ```bash
  source <VITIS_install_path>/settings64.sh
  source <XRT_install_path>/setup.sh
  ```

* Then make sure the following environment variable is correctly set to point to your U200 platform installation directory.

  ```bash
  export PLATFORM_REPO_PATHS=<path to the U250 platform install dir>
  ```

### Targeting Software Emulation
>**Deprecation Note**  
> * We are announcing the deprecation of the software emulation (sw_emu) feature in the Vitis Software Platform, which will affect all use cases, including embedded acceleration and data center applications.
> * Check the [AR000036790 - Deprecation of Software Emulation in Vitis Software Platform](https://adaptivesupport.amd.com/s/article/000036790?language=en_US) for detailed timelines and alternative solutions.

To build for software emulation, enter the following commands to setup the target build directory:

```bash
cd <Path to the cloned repo>/Getting_Started/Vitis/example/u250
mkdir sw_emu
cp xrt.ini sw_emu
```

Then, after changing into the target build directory, enter the following commands to build the host application and device binary:

```bash
g++ -g -std=c++17 -Wall -O0 ../../src/host.cpp -o ./app.exe -I$XILINX_XRT/include/ -L$XILINX_XRT/lib -lxrt_coreutil -pthread
emconfigutil --platform xilinx_u250_gen3x16_xdma_4_1_202210_1
v++ -t sw_emu -c -k vadd --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 --config ../src/u250.cfg -I../src ../src/vadd.cpp -o sw_emu/vadd.xo 
v++ -l -t sw_emu --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 --config ../src/u250.cfg ./sw_emu/vadd.xo -o ./sw_emu/vadd.xclbin
```

Here is a brief explanation of each of these four commands:

1. `g++` compiles the host application using the standard GNU C compiler. Refer to [Building the Software Application](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/Building-the-Software-Application) for more information.
2. `emconfigutil` generates an emulation configuration file which defines the device type and quantity of devices to emulate for the specified platform. Refer to [emconfigutil Utility](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/emconfigutil-Utility) for more information.
3. `v++ -c -k vadd` compiles the source code for the vector-add accelerator into a compiled kernel object (.xo file) for use in software emulation (sw_emu). Refer to [Compiling C/C++ PL Kernels for Software Emulation](https://docs.amd.com/access/sources/dita/topic?Doc_Version=2024.1%20English&url=ug1393-vitis-application-acceleration&resourceid=kqb1693589259529.html) for more information.
4. `v++ -l` links the compiled kernel with the target platform and generates the FPGA binary (.xclbin file). Refer to [Linking the System](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/Linking-the-System) for more information.

The `-t` option of the `v++` command specifies the build target. Here it is set to `sw_emu`, so you are building for software emulation.

Notice also the `--config` option which is used to specify the name of a configuration file containing additional options. Here you are using this configuration file to specify the number of kernels and the mapping of kernel arguments to specific memory banks.

```bash
debug=1
save-temps=1

[connectivity]
nk=vadd:1:vadd_1
sp=vadd_1.in1:DDR[1]
sp=vadd_1.in2:DDR[2]
sp=vadd_1.out:DDR[1]

[profile]
data=all:all:all
```

Building for software emulation is quick and should not take more than a few minutes. After the build process completes, you can launch the software emulation run as follows:

```bash
export XCL_EMULATION_MODE=sw_emu
./app.exe
```

You should see the following messages, indicating that the run completed successfully:

```bash
INFO: Found Xilinx Platform
INFO: Loading 'vadd.xclbin'
TEST PASSED
```

If you look at the directory contents for the `u250/sw_emu` directory, you should see some of the following files that were created during this exercise:

* **app.exe**: The compiled and linked host application.
* **emconfig.json**: The emulation platform created by emconfigutil.
* **vadd.xo**: The compiled Vadd kernel.
* **vadd.xo.compile_summary**: A summary report of the compiled kernel.
* **vadd.xclbin**: The device binary linking the kernel and target platform.
* **vadd.xclbin.info**: A text report of the device binary.
* **vadd.xclbin.link_summary**: A summary report of the linked device binary.
* **xrt.run_summary**: A summary report of the events of the application runtime.

These files and reports are the results of the build and run process targeting the software emulation build. You wil be taking a closer look at some of these files in Part 5 of this tutorial.

### Targeting Hardware Emulation

To build for hardware emulation, enter the following commands to setup the target build directory:

```bash
cd <Path to the cloned repo>/Getting_Started/Vitis/example/u250
mkdir hw_emu
cp xrt.ini hw_emu
```

Then, after changing into the target build directory, enter the following commands to build the host application and device binary:

```bash
g++ -g -std=c++17 -Wall -O0 ../../src/host.cpp -o ./app.exe -I$XILINX_XRT/include/ -L$XILINX_XRT/lib -lxrt_coreutil -pthread
emconfigutil --platform xilinx_u250_gen3x16_xdma_4_1_202210_1
v++ -c --mode hls --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 --config ./u250_hls.cfg --work_dir hw_emu
v++ -l -t hw_emu --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 --config ../src/u250.cfg ./hw_emu/vadd.xo -o ./hw_emu/vadd.xclbin
```

Building for hardware emulation takes more time than for software emulation, but still much less than when targeting the hardware accelerator card. Notice that the `v++ -c --mode hls` command replaces the `v++ -c -k vadd` command from the software emulation flow. After the build process completes, you can run hardware emulation using the following commands:

```bash
export XCL_EMULATION_MODE=hw_emu
./app.exe
```

When the run completes, you should see the TEST PASSED message indicating that the run completed successfully. You can list the contents of the `u250/hw_emu` folder to see the files that were created during this build and run process. Refer to *Targeting Software Emulation* for a brief explanation of the different files.

### Targeting Hardware

To build for the hardware target, enter the following commands to set up the target build directory:

```bash
cd <Path to the cloned repo>/Getting_Started/Vitis/example/u250
mkdir hw
cp xrt.ini hw
```

Then, after changing into the target build directory, enter the following commands to build the host application and device binary:

```bash
g++ -g -std=c++17 -Wall -O0 ../../src/host.cpp -o ./app.exe -I$XILINX_XRT/include/ -L$XILINX_XRT/lib -lxrt_coreutil -pthread
v++ -c --mode hls --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 --config ./u250_hls.cfg --work_dir hw
v++ -l -t hw --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 --config ../../src/u250.cfg ./hw/vadd.xo -o ./hw/vadd.xclbin
```

To target hardware, the `v++ -t` option is set to `hw` and the `emconfigutil` command is not needed, as you will be running on an actual hardware platform rather than an emulated platform. All other options remain identical.

Building for hardware targets can take a couple of hours, or more significant time depending on the design complexity, the machine you are building on, and its current workload.

After the build completes you can run the application on a system with the AMD Alveo™ U250 Data Center accelerator card using the following command:

```bash
./app.exe
```

>**NOTE:** Make sure to run the program on a server where the Alveo card is installed. If you built the application on a different machine, you will need to source the `/opt/xilinx/xrt/setup.sh` script after connecting to the desired server and before running the above command.

You should see the same TEST PASSED message indicating that the run completed successfully. If you look in the `u250/hw` folder, you will see some of the files that were created during this build and run process. Refer to *Targeting Software Emulation* for a brief explanation of the different files.

Congratulations!! You have just completed your first run of a Vitis accelerated application on the Alveo U250 card! There are additional [Vitis-Tutorials](https://github.com/Xilinx/Vitis-Tutorials) to work through to learn additional details of the Vitis tools, and [Vitis_Accel_Examples](https://github.com/Xilinx/Vitis_Accel_Examples) to use for examples of host application and kernel coding.

## Next Step

  **Click here to [Visualizing Results and Viewing Reports](./Part5.md)**

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
