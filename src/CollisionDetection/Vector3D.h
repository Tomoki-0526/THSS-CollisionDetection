#pragma once

class Vector3D
{
public:
	float x;
	float y;
	float z;

public:
	Vector3D();
	Vector3D(float x, float y, float z);

	float norm();

	void operator+=(const Vector3D& v);
	Vector3D operator-(const Vector3D& v);
	Vector3D operator-(const float diff);
	Vector3D operator*(const float scalar);
	float operator*(const Vector3D& v);
	Vector3D operator/(const float scalar);
	Vector3D operator-();
	Vector3D& operator=(const Vector3D& v);
};
