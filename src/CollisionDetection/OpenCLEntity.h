#pragma once

#include "const.h"
#include "utils.h"

class OpenCLEntity
{
public:
	cl_context context;				// �����ľ��
	cl_device_id device;			// �豸���
	cl_command_queue cmd_queue;		// ָ�����
	cl_program program;				// ������
	cl_kernel update_sphere;		// С���ƶ�������
	float platform_ver;				// OpenCL�汾
	float device_ver;				// �豸�汾
	float compiler_ver;				// �������汾

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

