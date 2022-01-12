#include "Camera.h"

Camera::Camera()
{
	eye.x = 2.0f;
	eye.y = 2.5f;
	eye.z = 1.75f;

	center.x = 0;
	center.y = 0;
	center.z = 0;

	up.x = 0;
	up.y = 1;
	up.z = 0;
}
