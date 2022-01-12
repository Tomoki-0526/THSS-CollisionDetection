#include "CollisionDetection.h"

/**
* OpenGL
*/
// 初始化小球
void initSpheres() {
	for (int i = 0; i < g_size; ++i) {
		int k = i;
		int x_k = k % g_mod;
		k = k / g_mod;
		int y_k = k % g_mod;
		k = k / g_mod;
		int z_k = k % g_mod;

		Vector3D a(x_k, y_k, z_k), b(y_k, z_k, x_k), c(z_k, x_k, y_k);
		spheres.x[i] = spheres.next_x[i] = x_k * 0.3 - 0.001 * y_k - 0.001 * z_k - 0.61;
		spheres.y[i] = spheres.next_y[i] = y_k * 0.3 - 0.001 * z_k - 0.001 * x_k - 0.61;
		spheres.z[i] = spheres.next_z[i] = z_k * 0.3 - 0.001 * x_k - 0.001 * y_k - 0.61;
		spheres.r[i] = i % 7 == 0 ? 0.15 : 0.1;
		spheres.m[i] = i % 5 == 0 ? 2 : 1;
		spheres.red[i] = (i * g_color) % 255;
		spheres.green[i] = ((i + 8) * g_color) % 255;
		spheres.blue[i] = ((i + 14) * g_color) % 255;
		spheres.shininess[i] = 62.5f;
		spheres.ambient_r[i] = 0.1579f;
		spheres.ambient_g[i] = 0.0215f;
		spheres.ambient_b[i] = 0.0846f;
		spheres.diffuse_r[i] = 0.4162f;
		spheres.diffuse_g[i] = 0.2104f;
		spheres.diffuse_b[i] = 0.0741f;
		spheres.specular_r[i] = 0.5003f;
		spheres.specular_g[i] = 0.0749f;
		spheres.specular_b[i] = 0.6246f;
		if (i % 11 == 0) {
			spheres.vx[i] = spheres.next_vx[i] = 0.003;
			spheres.vy[i] = spheres.next_vy[i] = -0.01;
			spheres.vz[i] = spheres.next_vz[i] = 0.002;
		}
		else {
			spheres.vx[i] = spheres.vy[i] = spheres.vz[i] = 0;
			spheres.next_vx[i] = spheres.next_vy[i] = spheres.next_vz[i] = 0;
		}
	}
}

// 绘制边界
void paintBound(const Vector3D& start, const Vector3D& length, const Color& color) {
	glPushMatrix();
	glColor3f(color.r, color.g, color.b);
	glTranslatef(start.x, start.y, start.z);
	glScalef(length.x, length.y, length.z);
	glutSolidCube(1);
	glPopMatrix();
}

// 绘制小球
void paintSphere(const Sphere& sphere) {
	glPushMatrix();
	glColor3f(sphere.color.r, sphere.color.g, sphere.color.b);
	glTranslatef(sphere.pos.x, sphere.pos.y, sphere.pos.z);
	glScalef(sphere.radius, sphere.radius, sphere.radius);
	glutSolidSphere(0.99, 15, 15);
	glPopMatrix();
}

void initializeGL()
{
	initSpheres();
}

void resizeGL(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)w / (GLfloat)h, 0.1, 300);
}

void paintGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z, 
		cam.center.x, cam.center.y, cam.center.z, 
		cam.up.x, cam.up.y, cam.up.z);
	glScalef(g_scalar, g_scalar, g_scalar);

	paintBound(Vector3D(0, -1, 0), Vector3D(2, 0, 2), Color(222, 184, 135));
	paintBound(Vector3D(0, 0, -1), Vector3D(2, 2, 0), Color(255, 222, 173));
	paintBound(Vector3D(-1, 0, 0), Vector3D(0, 2, 2), Color(250, 240, 230));

	for (int i = 0; i < g_size; ++i) {
		Sphere sphere(spheres.x[i], spheres.y[i], spheres.z[i], 
			spheres.r[i], spheres.m[i],
			spheres.vx[i], spheres.vy[i], spheres.vz[i],
			spheres.red[i], spheres.green[i], spheres.blue[i],
			spheres.shininess[i],
			spheres.ambient_r[i], spheres.ambient_g[i], spheres.ambient_b[i],
			spheres.diffuse_r[i], spheres.diffuse_g[i], spheres.diffuse_b[i],
			spheres.specular_r[i], spheres.specular_g[i], spheres.specular_b[i]);
		paintSphere(sphere);
	}
	glutSwapBuffers();
}

/**
* OpenCL
*/
// 释放cl代码文件
void releaseCl(char* src)
{
	if (src) {
		delete[] src;
		src = nullptr;
	}
}

// 获取cl代码创建program
cl_int CreateAndBuildProgram(OpenCLEntity* ocl)
{
	cl_int error = CL_SUCCESS;

	char* src = nullptr;
	size_t src_size = 0;
	error = ReadSourceFromFile("Template.cl", &src, &src_size);
	if (error != CL_SUCCESS) {
		LogError("Error: ReadSourceFromFile returned %s.\n", TranslateOpenCLError(error));
		releaseCl(src);
		return error;
	}

	ocl->program = clCreateProgramWithSource(ocl->context, 1, (const char**)&src, &src_size, &error);
	if (error != CL_SUCCESS) {
		LogError("Error: clCreateProgramWithSource returned %s.\n", TranslateOpenCLError(error));
		releaseCl(src);
		return error;
	}
	
	const char options[] = "";
	error = clBuildProgram(ocl->program, 1, &ocl->device, options, nullptr, nullptr);
	if (error != CL_SUCCESS) {
		LogError("Error: clBuildProgram() for source program returned %s.\n", TranslateOpenCLError(error));

		if (error == CL_BUILD_PROGRAM_FAILURE) {
			size_t log_size = 0;
			clGetProgramBuildInfo(ocl->program, ocl->device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);

			vector<char> build_log(log_size);
			clGetProgramBuildInfo(ocl->program, ocl->device, CL_PROGRAM_BUILD_LOG, log_size, &build_log[0], nullptr);

			LogError("Error happened during the build of OpenCL program.\nBuild log:%s", &build_log[0]);
		}
	}
}

// 从内存中创建OpenCL缓存
int CreateBufferArguments(OpenCLEntity* ocl)
{
	cl_int error = CL_SUCCESS;

	ocl->x = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.x, &error);
	ocl->y = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.y, &error);
	ocl->z = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.z, &error);
	ocl->vx = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.vx, &error);
	ocl->vy = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.vy, &error);
	ocl->vz = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.vz, &error);
	ocl->r = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.r, &error);
	ocl->m = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.m, &error);
	ocl->nx = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.next_x, &error);
	ocl->ny = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.next_y, &error);
	ocl->nz = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.next_z, &error);
	ocl->nvx = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.next_vx, &error);
	ocl->nvy = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.next_vy, &error);
	ocl->nvz = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, g_size * sizeof(float), &spheres.next_vz, &error);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clCreateImage for srcA returned %s\n", TranslateOpenCLError(error));
		return error;
	}
	return CL_SUCCESS;
}

// 设置kernel参数
cl_uint SetKernelArguments(OpenCLEntity* ocl)
{
	cl_int error = CL_SUCCESS;

	error += clSetKernelArg(ocl->update_sphere, 0, sizeof(cl_mem), &ocl->x);
	error += clSetKernelArg(ocl->update_sphere, 1, sizeof(cl_mem), &ocl->y);
	error += clSetKernelArg(ocl->update_sphere, 2, sizeof(cl_mem), &ocl->z);
	error += clSetKernelArg(ocl->update_sphere, 3, sizeof(cl_mem), &ocl->vx);
	error += clSetKernelArg(ocl->update_sphere, 4, sizeof(cl_mem), &ocl->vy);
	error += clSetKernelArg(ocl->update_sphere, 5, sizeof(cl_mem), &ocl->vz);
	error += clSetKernelArg(ocl->update_sphere, 6, sizeof(cl_mem), &ocl->r);
	error += clSetKernelArg(ocl->update_sphere, 7, sizeof(cl_mem), &ocl->m);

	error += clSetKernelArg(ocl->update_sphere, 8, sizeof(cl_mem), &ocl->nx);
	error += clSetKernelArg(ocl->update_sphere, 9, sizeof(cl_mem), &ocl->ny);
	error += clSetKernelArg(ocl->update_sphere, 10, sizeof(cl_mem), &ocl->nz);
	error += clSetKernelArg(ocl->update_sphere, 11, sizeof(cl_mem), &ocl->nvx);
	error += clSetKernelArg(ocl->update_sphere, 12, sizeof(cl_mem), &ocl->nvy);
	error += clSetKernelArg(ocl->update_sphere, 13, sizeof(cl_mem), &ocl->nvz);
	error += clSetKernelArg(ocl->update_sphere, 14, sizeof(int), &g_size);

	if (error != CL_SUCCESS)
	{
		LogError("error: Failed to set argument, returned %s\n", TranslateOpenCLError(error));
		return error;
	}
	return error;
}

// 运行kernel
cl_uint ExecuteKernel(OpenCLEntity* ocl)
{
	cl_int error = CL_SUCCESS;
	// Define global iteration space for clEnqueueNDRangeKernel.
	size_t work_size = g_size;
	// execute kernel
	error = clEnqueueNDRangeKernel(ocl->cmd_queue, ocl->update_sphere, 1, nullptr, &work_size, nullptr, 0, nullptr, nullptr);
	if (error != CL_SUCCESS)
	{
		LogError("Error: Failed to run kernel, return %s\n", TranslateOpenCLError(error));
		return error;
	}
	// Wait until the queued kernel is completed by the device
	error = clFinish(ocl->cmd_queue);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clFinish return %s\n", TranslateOpenCLError(error));
		return error;
	}
	return CL_SUCCESS;
}

// OpenCL buffer到内存的映射
bool ReadAndVerify(OpenCLEntity* ocl)
{
	cl_int error = CL_SUCCESS;
	bool result = true;

	error = clEnqueueReadBuffer(ocl->cmd_queue, ocl->nx, CL_TRUE, 0, sizeof(float) * g_size, spheres.x, 0, nullptr, nullptr);
	error = clEnqueueReadBuffer(ocl->cmd_queue, ocl->ny, CL_TRUE, 0, sizeof(float) * g_size, spheres.y, 0, nullptr, nullptr);
	error = clEnqueueReadBuffer(ocl->cmd_queue, ocl->nz, CL_TRUE, 0, sizeof(float) * g_size, spheres.z, 0, nullptr, nullptr);
	error = clEnqueueReadBuffer(ocl->cmd_queue, ocl->nvx, CL_TRUE, 0, sizeof(float) * g_size, spheres.vx, 0, nullptr, nullptr);
	error = clEnqueueReadBuffer(ocl->cmd_queue, ocl->nvy, CL_TRUE, 0, sizeof(float) * g_size, spheres.vy, 0, nullptr, nullptr);
	error = clEnqueueReadBuffer(ocl->cmd_queue, ocl->nvz, CL_TRUE, 0, sizeof(float) * g_size, spheres.vz, 0, nullptr, nullptr);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clEnqueueMapBuffer returned %s\n", TranslateOpenCLError(error));
		return false;
	}

	error = clFinish(ocl->cmd_queue);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clFinish returned %s\n", TranslateOpenCLError(error));
	}
	return result;
}

// 检查OpenCL平台
bool CheckPreferredPlatformMatch(cl_platform_id platform, const char* preferredPlatform)
{
	size_t stringLength = 0;
	cl_int error = CL_SUCCESS;
	bool match = false;

	// In order to read the platform's name, we first read the platform's name string length (param_value is nullptr).
	// The value returned in stringLength
	error = clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, nullptr, &stringLength);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetPlatformInfo() to get CL_PLATFORM_NAME length returned '%s'.\n", TranslateOpenCLError(error));
		return false;
	}

	// Now, that we know the platform's name string length, we can allocate enough space before read it
	std::vector<char> platformName(stringLength);

	// Read the platform's name string
	// The read value returned in platformName
	error = clGetPlatformInfo(platform, CL_PLATFORM_NAME, stringLength, &platformName[0], nullptr);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetplatform_ids() to get CL_PLATFORM_NAME returned %s.\n", TranslateOpenCLError(error));
		return false;
	}

	// Now check if the platform's name is the required one
	if (strstr(&platformName[0], preferredPlatform) != 0)
	{
		// The checked platform is the one we're looking for
		LogInfo("Platform: %s\n", &platformName[0]);
		match = true;
	}

	return match;
}

// 获取OpenCL平台
cl_platform_id FindOpenCLPlatform(const char* preferredPlatform, cl_device_type deviceType)
{
	cl_uint numPlatforms = 0;
	cl_int error = CL_SUCCESS;

	// Get (in numPlatforms) the number of OpenCL platforms available
	// No platform ID will be return, since platforms is nullptr
	error = clGetPlatformIDs(0, nullptr, &numPlatforms);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetplatform_ids() to get num platforms returned %s.\n", TranslateOpenCLError(error));
		return nullptr;
	}
	LogInfo("Number of available platforms: %u\n", numPlatforms);

	if (0 == numPlatforms)
	{
		LogError("Error: No platforms found!\n");
		return nullptr;
	}

	std::vector<cl_platform_id> platforms(numPlatforms);

	// Now, obtains a list of numPlatforms OpenCL platforms available
	// The list of platforms available will be returned in platforms
	error = clGetPlatformIDs(numPlatforms, &platforms[0], nullptr);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetplatform_ids() to get platforms returned %s.\n", TranslateOpenCLError(error));
		return nullptr;
	}

	// Check if one of the available platform matches the preferred requirements
	for (cl_uint i = 0; i < numPlatforms; i++)
	{
		bool match = true;
		cl_uint numDevices = 0;

		// If the preferredPlatform is not nullptr then check if platforms[i] is the required one
		// Otherwise, continue the check with platforms[i]
		if ((nullptr != preferredPlatform) && (strlen(preferredPlatform) > 0))
		{
			// In case we're looking for a specific platform
			match = CheckPreferredPlatformMatch(platforms[i], preferredPlatform);
		}

		// match is true if the platform's name is the required one or don't care (nullptr)
		if (match)
		{
			// Obtains the number of deviceType devices available on platform
			// When the function failed we expect numDevices to be zero.
			// We ignore the function return value since a non-zero error code
			// could happen if this platform doesn't support the specified device type.
			error = clGetDeviceIDs(platforms[i], deviceType, 0, nullptr, &numDevices);
			if (error != CL_SUCCESS)
			{
				LogInfo("   Required device was not found on this platform.\n");
			}

			if (0 != numDevices)
			{
				// There is at list one device that answer the requirements
				LogInfo("   Required device was found.\n");
				return platforms[i];
			}
		}
	}

	LogError("Error: Required device was not found on any platform.\n");
	return nullptr;
}

// 存储OpenCL平台和版本号
int GetPlatformAndDeviceVersion(cl_platform_id platformId, OpenCLEntity* ocl)
{
	cl_int error = CL_SUCCESS;

	// Read the platform's version string length (param_value is nullptr).
	// The value returned in stringLength
	size_t stringLength = 0;
	error = clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, 0, nullptr, &stringLength);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetPlatformInfo() to get CL_PLATFORM_VERSION length returned '%s'.\n", TranslateOpenCLError(error));
		return error;
	}

	// Now, that we know the platform's version string length, we can allocate enough space before read it
	std::vector<char> platformVersion(stringLength);

	// Read the platform's version string
	// The read value returned in platformVersion
	error = clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, stringLength, &platformVersion[0], nullptr);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetplatform_ids() to get CL_PLATFORM_VERSION returned %s.\n", TranslateOpenCLError(error));
		return error;
	}

	if (strstr(&platformVersion[0], "OpenCL 2.0") != nullptr)
	{
		ocl->platform_ver = OPENCL_VERSION_2_0;
	}

	// Read the device's version string length (param_value is nullptr).
	error = clGetDeviceInfo(ocl->device, CL_DEVICE_VERSION, 0, nullptr, &stringLength);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetDeviceInfo() to get CL_DEVICE_VERSION length returned '%s'.\n", TranslateOpenCLError(error));
		return error;
	}

	// Now, that we know the device's version string length, we can allocate enough space before read it
	std::vector<char> deviceVersion(stringLength);

	// Read the device's version string
	// The read value returned in deviceVersion
	error = clGetDeviceInfo(ocl->device, CL_DEVICE_VERSION, stringLength, &deviceVersion[0], nullptr);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetDeviceInfo() to get CL_DEVICE_VERSION returned %s.\n", TranslateOpenCLError(error));
		return error;
	}

	if (strstr(&deviceVersion[0], "OpenCL 2.0") != nullptr)
	{
		ocl->device_ver = OPENCL_VERSION_2_0;
	}

	// Read the device's OpenCL C version string length (param_value is nullptr).
	error = clGetDeviceInfo(ocl->device, CL_DEVICE_OPENCL_C_VERSION, 0, nullptr, &stringLength);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetDeviceInfo() to get CL_DEVICE_OPENCL_C_VERSION length returned '%s'.\n", TranslateOpenCLError(error));
		return error;
	}

	// Now, that we know the device's OpenCL C version string length, we can allocate enough space before read it
	std::vector<char> compilerVersion(stringLength);

	// Read the device's OpenCL C version string
	// The read value returned in compilerVersion
	error = clGetDeviceInfo(ocl->device, CL_DEVICE_OPENCL_C_VERSION, stringLength, &compilerVersion[0], nullptr);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetDeviceInfo() to get CL_DEVICE_OPENCL_C_VERSION returned %s.\n", TranslateOpenCLError(error));
		return error;
	}

	else if (strstr(&compilerVersion[0], "OpenCL C 2.0") != nullptr)
	{
		ocl->compiler_ver = OPENCL_VERSION_2_0;
	}

	cout << "PlatforVersion:" << ocl->platform_ver << "  deviceVersion:" << ocl->device_ver << "  CompilerVersion:" << ocl->compiler_ver << endl;
	return error;
}

// OpenCL预处理
int SetupOpenCL(OpenCLEntity* ocl, cl_device_type deviceType)
{
	// The following variable stores return codes for all OpenCL calls.
	cl_int error = CL_SUCCESS;

	// Query for all available OpenCL platforms on the system
	// Here you enumerate all platforms and pick one which name has preferredPlatform as a sub-string
	cl_platform_id platformId = FindOpenCLPlatform("Intel", deviceType);
	if (nullptr == platformId)
	{
		LogError("Error: Failed to find OpenCL platform.\n");
		return CL_INVALID_VALUE;
	}

	// Create context with device of specified type.
	// Required device type is passed as function argument deviceType.
	// So you may use this function to create context for any CPU or GPU OpenCL device.
	// The creation is synchronized (pfn_notify is nullptr) and nullptr user_data
	cl_context_properties contextProperties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platformId, 0 };
	ocl->context = clCreateContextFromType(contextProperties, deviceType, nullptr, nullptr, &error);
	if ((error != CL_SUCCESS) || (nullptr == ocl->context))
	{
		LogError("Couldn't create a context, clCreateContextFromType() returned '%s'.\n", TranslateOpenCLError(error));
		return error;
	}

	// Query for OpenCL device which was used for context creation
	error = clGetContextInfo(ocl->context, CL_CONTEXT_DEVICES, sizeof(cl_device_id), &ocl->device, nullptr);
	if (error != CL_SUCCESS)
	{
		LogError("Error: clGetContextInfo() to get list of devices returned %s.\n", TranslateOpenCLError(error));
		return error;
	}

	// Read the OpenCL platform's version and the device OpenCL and OpenCL C versions
	GetPlatformAndDeviceVersion(platformId, ocl);

	// Create command queue.
	// OpenCL kernels are enqueued for execution to a particular device through special objects called command queues.
	// Command queue guarantees some ordering between calls and other OpenCL commands.
	// Here you create a simple in-order OpenCL command queue that doesn't allow execution of two kernels in parallel on a target device.
#ifdef CL_VERSION_2_0
	if (OPENCL_VERSION_2_0 == ocl->device_ver)
	{
		const cl_command_queue_properties properties[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };
		ocl->cmd_queue = clCreateCommandQueueWithProperties(ocl->context, ocl->device, properties, &error);
	}
	else {
		// default behavior: OpenCL 1.2
		cl_command_queue_properties properties = CL_QUEUE_PROFILING_ENABLE;
		ocl->cmd_queue = clCreateCommandQueue(ocl->context, ocl->device, properties, &error);
	}
#else
	// default behavior: OpenCL 1.2
	cl_command_queue_properties properties = CL_QUEUE_PROFILING_ENABLE;
	ocl->commandQueue = clCreateCommandQueue(ocl->context, ocl->device, properties, &error);
#endif
	if (error != CL_SUCCESS)
	{
		LogError("Error: clCreateCommandQueue() returned %s.\n", TranslateOpenCLError(error));
		return error;
	}

	return CL_SUCCESS;
}

void update() {
	ExecuteKernel(&ocl);
	ReadAndVerify(&ocl);
}

void timer(int id) {
	if (cnt == 200) {
		end_time = clock();
		printf("Time: %f(ms)\n", (double)(end_time - start_time));
	}

	update();
	glutPostRedisplay();
	glutTimerFunc(g_timeInterval, timer, 1);

	cnt++;
}

/**
* 主函数
*/
int _tmain(int argc, TCHAR** argv) {
	start_time = clock();

	initializeGL();

	cl_int error;
	cl_device_type deviceType = CL_DEVICE_TYPE_GPU;

	if (CL_SUCCESS != SetupOpenCL(&ocl, deviceType))
	{
		return -1;
	}
	if (CL_SUCCESS != CreateBufferArguments(&ocl))
	{
		return -1;
	}
	if (CL_SUCCESS != CreateAndBuildProgram(&ocl))
	{
		return -1;
	}

	ocl.update_sphere = clCreateKernel(ocl.program, "updateSphere", &error);
	if (CL_SUCCESS != error)
	{
		LogError("Error: clCreateKernel returned %s\n", TranslateOpenCLError(error));
		return -1;
	}

	if (CL_SUCCESS != SetKernelArguments(&ocl))
	{
		return -1;
	}

	char** _argv = nullptr;
	glutInit(&argc, _argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1600, 900);
	glutInitWindowPosition(100, 80);
	glutCreateWindow("GPU Collision Detection");
	glEnable(GL_DEPTH_TEST);
	glutTimerFunc(g_timeInterval, timer, 1);
	glutReshapeFunc(resizeGL);
	glutDisplayFunc(paintGL);
	glutMainLoop();
	return 0;
}