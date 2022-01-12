#include "OpenCLEntity.h"

OpenCLEntity::OpenCLEntity() :
	context(nullptr),
	device(nullptr),
	cmd_queue(nullptr),
	program(nullptr),
	update_sphere(nullptr),
	platform_ver(OPENCL_VERSION_1_2),
	device_ver(OPENCL_VERSION_1_2),
	compiler_ver(OPENCL_VERSION_1_2),
	size(125),
	x(nullptr), y(nullptr), z(nullptr), nx(nullptr), ny(nullptr), nz(nullptr),
	vx(nullptr), vy(nullptr), vz(nullptr), nvx(nullptr), nvy(nullptr), nvz(nullptr),
	r(nullptr), m(nullptr)
{
}

OpenCLEntity::~OpenCLEntity()
{
	cl_int error = CL_SUCCESS;

	if (program) {
		error = clReleaseProgram(program);
		if (error != CL_SUCCESS) {
			LogError("Error: Release error: '%s'.\n", TranslateOpenCLError(error));
		}
	}
	if (context)
	{
		error = clReleaseContext(context);
		if (error != CL_SUCCESS) {
			LogError("Error: Release error: '%s'.\n", TranslateOpenCLError(error));
		}
	}
	if (cmd_queue)
	{
		error = clReleaseCommandQueue(cmd_queue);
		if (error != CL_SUCCESS) {
			LogError("Error: Release error: '%s'.\n", TranslateOpenCLError(error));
		}
	}
	if (update_sphere) {
		error = clReleaseKernel(update_sphere);
		if (error != CL_SUCCESS) {
			LogError("Error: Release error: '%s'.\n", TranslateOpenCLError(error));
		}
	}
	if (device)
	{
		error = clReleaseDevice(device);
		if (error != CL_SUCCESS) {
			LogError("Error: Release error: '%s'.\n", TranslateOpenCLError(error));
		}
	}
}
