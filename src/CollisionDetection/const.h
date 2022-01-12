#pragma once
#pragma warning( disable : 4996 )

#include "CL\cl.h"

// Macros for OpenCL versions
#define OPENCL_VERSION_1_2  1.2f
#define OPENCL_VERSION_2_0  2.0f

// Suppress a compiler warning about undefined CL_TARGET_OPENCL_VERSION
// Khronos ICD supports only latest OpenCL version
#define CL_TARGET_OPENCL_VERSION 220

// Suppress a compiler warning about 'clCreateCommandQueue': was declared deprecated
// for OpenCL 1.2
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

const int g_size = 100;				// 小球数量
const int g_timeInterval = 10;		// 更新时间间隔

const int g_mod = 4;
const int g_color = 42;