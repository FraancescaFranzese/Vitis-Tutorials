# Bilinear Interpolation on AIE-ML

***Version: Vitis 2024.1***

## Table of Contents

1. [Introduction](#introduction)
2. [Kernel Code Modifications to target AIE-ML](#kernel-code-modifications-to-target-aie-ml)
7. [Running the Example](#running-the-example)
8. [Analyzing Results](#analyzing-results)
9. [Customizing the Example](#customizing-the-example)

[References](#references)

## Introduction

Bilinear interpolation is a method for interpolating functions of two variables using repeated linear interpolation. It is commonly used in the following applications:

- Image processing and computer vision, where it is used to resample images and textures. For example, when zooming in or out of an image, bilinear interpolation can be used to estimate color values of new pixels based on color values of the original pixels. [[1]] [[2]]
- Finite element analysis, where it is used to estimate values of variables such as stress and strain between nodes in a finite element mesh. [[3]]
- Geographic information systems, where it is used to interpolate elevation or other spatial data from a grid of points. [[4]]
- Computer graphics, where it is used to map textures onto 3D surfaces or perform texture filtering. [[5]]

Bilinear interpolation is one of the simplest and fastest interpolation methods, but it can also introduce some artifacts such as blurring or aliasing. More advanced interpolation methods, such as bicubic interpolation or spline interpolation, can produce smoother and more accurate results, but they are also more computationally expensive.

This tutorial targets AIE-ML device, but shares test cases and description files with AIE [11-Bilinear_Interpolation](../../../AIE/Design_Tutorials/11-Bilinear_Interpolation).

### AIE-ML Engine Input and Output Data Type
This example is available in two versions:
* The first version uses MATLAB® to generate test vectors, which are sequences of `int32` numbers. Although actual data is single precision floating-point, it is difficult to express such numbers in text format. In order to capture full precision, the 32 bits used to represent a floating-point number (sign, exponent, mantissa) are written as equivalent `int32` values. A similar format is used for files containing output data.
* The second version uses MATLAB® to generate test vectors, as sequences of `int16` numbers. This idea was experimented because 32-bit floating-point vector data path is not directly supported by AIE-ML processor, but can be emulated via decomposition into multiple multiplications of 16 x 16-bit. As a result, the float-based design experiences a performance loss compared to the AIE case. Therefore, the idea was to test a version using the int16 format, which is much faster at the cost of reduced result accuracy.
Therefore, the tutorial is divided into two independent directories that show the two developed cases.

## Kernel Code Modifications to target AIE-ML

The following outlines the main modifications made to make the code compatible with AIE-ML compilation. 

In the first case, the AIE API library calls have been used. AIE API is a portable programming interface for AIE accelerators. It is implemented as a C++ header-only library that provides types and operations that get translated into efficient low-level intrinsics. The API also provides higher-level abstractions such as iterators and multi-dimensional arrays. [[6]].
First, instead of using the keyword `auto`, vectors and accumulators must be explicitly declared: 

```cpp

	// get data for first x interpolation
	aie::vector<float, 8> xfrac = (*pInA++).cast_to<float>();
	aie::vector<float, 8> p11 = (*pInB++).cast_to<float>();
	aie::vector<float, 8> p21 = (*pInC++).cast_to<float>();

```
The function `fpmac()` is not supported for AIE-ML, as it belongs to the intrinsic AIE APIs, which are strictly dependent on the device's hardware. Therefore, it will be replaced with the AIE API calls `aie::mac()` and `aie::msc()`. The latter take as input the accumulator to which the result of the multiplication is added or subtracted, and the two vectors to be multiplied. Therefore, the code also includes some conversions to transform vectors into accumulators and vice versa.


```cpp

	aie::accum<accfloat, 8> p11_acc;
        p11_acc.from_vector(p11);

	// compute first x interpolation
	aie::accum<accfloat, 8> tempy1 = aie::mac(p11_acc,xfrac,p21); 
	aie::accum<accfloat, 8> pxy1 = aie::msc(tempy1, xfrac, p11); 

```

In the case with the int16 data type, the modifications are very similar. However, instead of using the AIE engine APIs, the intrinsic calls `mac()` and `msc()` have been used:
```cpp

	// get data for first x interpolation
	aie::vector<int16,8> xfrac = *pInA++;
	aie::vector<int16,8> p11 = *pInB++;
	aie::vector<int16,8> p21 = *pInC++;

	aie::accum<acc32, 8> p11_acc;
	p11_acc.from_vector(p11);

	// compute first x interpolation
	auto tempy1 = mac(p11_acc,xfrac,p21);
	auto pxy1 = msc(tempy1,xfrac,p11);

```


## Running the Example

Running the example requires that both MATLAB and AMD Vitis™ tools are installed and configured correctly. After downloading the files, cd into the ``.../bilinear_interpolation/aie/`` directory and use the make build process.

### Generating Test Vectors

Prior to running the AI Engine graph simulation, test vectors are required to provide input. Files are also provided to compare with AI Engine simulator output for verification. To generate the vectors, run the command:

```bash
$ make gen_vectors
```

### Running x86 Simulation

To perform a functional x86 simulation, enter the following sequence of commands:

```bash
$ make x86com
$ make x86sim
$ make check_x86sim
```

The first command compiles graph code for simulation on an x86 processor, the second command runs the simulation, and the final command invokes MATLAB to compare the simulator output with test vectors.

### Running AI Engine Simulation

To perform AI Engine emulation using the SystemC simulator, enter the following sequence of commands:

```bash
$ make aiecom
$ make aiesim
$ make check_aiesim
```

The first command compiles graph code for the SystemC simulator, the second command runs the simulation, and the final command invokes MATLAB to compare simulation output with test vectors. If it is desired to generate trace and profile data during simulation, use the sequence:

```bash
$ make aiecom
$ make profile
$ make check_aiesim
```

## Analyzing Results

### Vitis Analyzer

Vitis Analyzer is an essential tool for accessing information on compilation, simulation, and implementation of AI Engine graphs. It can be used to obtain a summary on profiling data and to graphically display trace events. The tool may be invoked with the ``vitis_analyzer`` command, or for this example, by entering:

```bash
$ make analyze
```


## Customizing the example

### Specifying a Test Image and Output Resolution

The ``image_transform`` function uses file ``../images/epyc.jpg`` as a test image by default. A different file named ``image_file`` may be specified when invoking the function in MATLAB. 

```bash
>> image_transform('image_file')
```

Default output resolution in pixels is 1024 $\times$ 1024. A different resolution of $x_{res} \times y_{res}$ may be specified by invoking

```bash
>> image_transform('image_file', [ xres  yres ])
```

The ``image_transform`` function reads the test image file, performs a sequence of linear transformations on image pixel coordinates, and stores these transformations in a file named ``bli_coords.mat`` along with the test image pixel array. When this function is executed in MATLAB, a sequence of image transformations are displayed.


## References

[1]:<https://en.wikipedia.org/wiki/Bilinear_interpolation> "Bilinear interpolation"

[[1]]: Bilinear interpolation. 17 May 2023, 14:05 UTC. In Wikipedia: The Free Encyclopedia. Wikimedia Foundation Inc. Encyclopedia on-line. Available from https://en.wikipedia.org/wiki/Bilinear_interpolation. Internet. Retrieved 4 August 2023.

[2]: <https://ieeexplore.ieee.org/document/1409828> "A novel approach to real-time bilinear interpolation"
[[2]]: K. T. Gribbon and D. G. Bailey, "A novel approach to real-time bilinear interpolation," Proceedings. DELTA 2004. Second IEEE International Workshop on Electronic Design, Test and Applications, Perth, WA, Australia, 2004, pp. 126-131, doi: 10.1109/DELTA.2004.10055.

[3]: <https://en.thedailyscoup.news/2023/03/05/interpolation-formula-understanding-the-basics-and-applications/> "Interpolation Formula: Understanding the Basics and Applications"

[[3]] Chen, J. (2023, March 5). *Interpolation Formula: Understanding the Basics and Applications.* The Daily Scoup News. https://en.thedailyscoup.news/2023/03/05/interpolation-formula-understanding-the-basics-and-applications/

[4]: <https://ieeexplore.ieee.org/document/9943113> "Super-Resolution of Digital Elevation Model with Local Implicit Function Representation"

[[4]] P. He et al., "Super-Resolution of Digital Elevation Model with Local Implicit Function Representation," 2022 International Conference on Machine Learning and Intelligent Systems Engineering (MLISE), Guangzhou, China, 2022, pp. 111-116, doi: 10.1109/MLISE57402.2022.00030.

[5]: <https://bartwronski.com/2020/04/14/bilinear-texture-filtering-artifacts-alternatives-and-frequency-domain-analysis/> "Bilinear texture filtering – artifacts, alternatives, and frequency domain analysis"

[[5]] Wronski, B. (2020, April 14). *Bilinear texture filtering – artifacts, alternatives, and frequency domain analysis.* https://bartwronski.com/2020/04/14/bilinear-texture-filtering-artifacts-alternatives-and-frequency-domain-analysis/

[6]: <https://www.xilinx.com/htmldocs/xilinx2024_2/aiengine_api/aie_api/doc/index.html> "AI Engine API User Guide (AIE-API) 2024.2 - OverView"

[[6]] AI Engine API User Guide (AIE-API) 2024.2 - OverView* https://www.xilinx.com/htmldocs/xilinx2024_2/aiengine_api/aie_api/doc/index.html

