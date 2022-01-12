#pragma once

#include "Vector3D.h"

class Camera
{
public:
	Camera();

public:
	Vector3D eye;
	Vector3D center;
	Vector3D up;
};
