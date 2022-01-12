#pragma once

#include "Vector3D.h"
#include "Color.h"

class Sphere
{
public:
	Vector3D pos;
	float radius;
	Vector3D velocity;
	float mass;
	Color color;

	Vector3D next_pos;
	Vector3D next_velocity;

	Color ambient;
	Color diffuse;
	Color specular;
	float shininess;

	Sphere(float x, float y, float z, 
		float r, float m, 
		float vx, float vy, float vz, 
		float red, float green, float blue, 
		float shininess, 
		float ambient_r, float ambient_g, float ambient_b,
		float diffuse_r, float diffuse_g, float diffuse_b,
		float specular_r, float specular_g, float specular_b);
};

