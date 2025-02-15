﻿<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis Platform Creation Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See AMD Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Versal Custom Platform Creation Tutorial

***Version: 2024.2***

In this module, we treat the VCK190 board as a customized solution tailored to our customer's specific needs. We leverage the AMD Versal™ Adaptive SoC Extensible Part Support Example Design, a pre-built Linux common image, and the Device Tree Blob generated during the platform creation process to produce essential software components. These components form the foundation of a dedicated embedded Versal acceleration platform tailored for the customer’s board. Finally, you will test this platform by running several applications.

If your goal is simply to create a platform for kernel validation, then the [Vitis Platform Quick Start](https://docs.amd.com/r/en-US/Vitis-Tutorials-Getting-Started/Vitis-Platform) offers a streamlined approach.

For your reference, the overall structure of this example system closely resembles the following:

![Vitis Platform structure](images/structure.svg)

In a general Vitis acceleration platform design, the Vitis platform and application development can be divided into these parts:

1. Platform hardware design creation in the AMD Vivado™ Design Suite. It exports an XSA file with clock, reset, AXI interface, and interrupt signals and properties.
2. Platform software preparation with common image or using PetaLinux tool, including Linux kernel, rootfs, device tree, and boot components.
3. Platform creation in Vitis to combine all hardware and software components and generate XPFM description.
4. Create applications in Vitis against the platform. Vitis generates a host application, xclbin, and `sd_card.img`.
5. Write `sd_card.img` to a SD card or update host application and xclbin to an existing SD card.

In this module, you will utilize the Versal Extensible Part Support Design (CED) to create a hardware design. In contrast to the Versal Extensible Design, which is used in [Vitis Platform Quick Start](https://docs.amd.com/r/en-US/Vitis-Tutorials-Getting-Started/Vitis-Platform), the part support design takes a slightly different approach. Specifically, it empowers you to handle board-level configurations independently. This includes configuring processing system (PS) side peripherals and fine-tuning parameters related to double data rate (DDR) as needed. This flexibility ensures that you have full control over the configuration process to meet the unique requirements of your application.


To prepare the software components, you will use the common image provided by AMD. The Device Tree Blob (DTB) will be generated automatically during the platform creation process if the DTB generation option is enabled. Once both the software and hardware components are ready, you will proceed to package the platform.

In each step, you will validate the generated files to make sure they work as expected. A frequent test methodology can help to narrow down the root causes if any error occurs. Lastly, you will run several test applications on this platform to test this platform.

The total flow is similar the following:

![Vitis Platform structure](images/flow.svg)

Navigate through these steps with the following table of contents links.

- [Step 1: Create a Hardware Design](./step1.md)
- [Step 2: Create a Vitis Platform](./step2.md)
- [Step 3: Run Applications on the Vitis Platform](./step3.md)
- [Iteration Guidelines](./Iteration_guideline.md)
- [Frequently Asked Questions](faq.md)

## Reference

- [UG1701: Vitis Acceleration Flow User Guide](https://docs.amd.com/r/en-US/ug1701-vitis-accelerated-embedded/Developing-Vitis-Kernels-and-Applications)
- [Vitis Embedded Platform Source Github Repository](https://github.com/Xilinx/Vitis_Embedded_Platform_Source)
- [Versal Adaptive SoC Programmable Network on Chip and Integrated Memory Controller LogiCORE IP Product Guide (PG313)](https://docs.amd.com/r/en-US/pg313-network-on-chip)

## Changelog
### 2024.2
- The Platform Creation Wizard now includes an option to generate the DTB during platform creation, replacing the createdts command used in previous release.
### 2022.2

- `createdts` add support for your device tree.

### 2022.1

- From this version, you begin to use pre-built Linux common image instead of building Linux components with PetaLinux because the pre-built common image provides most common features for Vitis acceleration, and it can help expedite the platform creation process. If you wish to leverage the PetaLinux tool to customize your system, refer to the [PetaLinux Customization Tutorial](../../Feature_Tutorials/02_petalinux_customization/README.md).
- Simplify the device tree generation flow by using the newly introduced `createdts` XSCT command.
- Add validation for every step's output.

### 2021.2

- In Step 1, the Vivado project exports the XSA for hardware and hardware emulation seperatedly. When creating the platform in [step3](./step3.md), it is recommended to provide seperate XSA for hardware and hardware emulation.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
