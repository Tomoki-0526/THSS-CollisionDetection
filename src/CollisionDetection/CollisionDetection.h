/*****************************************************************************
 * Copyright (c) 2013-2016 Intel Corporation
 * All rights reserved.
 *
 * WARRANTY DISCLAIMER
 *
 * THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
 * MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Intel Corporation is the author of the Materials, and requests that all
 * problem reports or change requests be submitted to it directly
 *****************************************************************************/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <memory.h>
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>
#include <GL\glut.h>
#include <gl\freeglut.h>
#include <gl\freeglut_ext.h>
#include <gl\freeglut_std.h>
#include <ctime>

#include "CL\cl.h"
#include "utils.h"
#include "const.h"
#include "Camera.h"
#include "Vector3D.h"
#include "Sphere.h"
#include "Spheres.h"
#include "Color.h"
#include "OpenCLEntity.h"

using namespace std;

Spheres spheres;					// 全体小球
float g_scalar = 1.0f;				// 缩放比例
Camera cam;							// 相机

int cnt = 0;
clock_t start_time, end_time;

OpenCLEntity ocl;