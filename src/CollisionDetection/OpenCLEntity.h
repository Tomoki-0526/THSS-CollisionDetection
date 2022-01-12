#pragma once

#include "const.h"
#include "utils.h"

class OpenCLEntity
{
public:
	cl_context context;				// 上下文句柄
	cl_device_id device;			// 设备句柄
	cl_command_queue cmd_queue;		// 指令队列
	cl_program program;				// 程序句柄
	cl_kernel update_sphere;		// 小球移动控制器
	float platform_ver;				// OpenCL版本
	float device_ver;				// 设备版本
	float compiler_ver;				// 编译器版本

	cl_mem x;
	cl_mem y;
	cl_mem z;
	cl_mem r;
	cl_mem vx;
	cl_mem vy;
	cl_mem vz;
	cl_mem m;

	cl_mem nx;
	cl_mem ny;
	cl_mem nz;
	cl_mem nvx;
	cl_mem nvy;
	cl_mem nvz;
	size_t size;

	OpenCLEntity();
	~OpenCLEntity();
};

