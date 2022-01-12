#include "Sphere.h"

Sphere::Sphere(float x, float y, float z, 
	float r, float m, 
	float vx, float vy, float vz, 
	float red, float green, float blue, 
	float shininess, 
	float ambient_r, float ambient_g, float ambient_b, 
	float diffuse_r, float diffuse_g, float diffuse_b, 
	float specular_r, float specular_g, float specular_b) :
	pos(Vector3D(x, y, z)),
	radius(r),
	velocity(Vector3D(vx, vy, vz)),
	mass(m),
	color(Color(red, green, blue)),
	shininess(shininess),
	ambient(Color(ambient_r, ambient_g, ambient_b)),
	diffuse(Color(diffuse_r, diffuse_g, diffuse_b)),
	specular(Color(specular_r, specular_g, specular_b))
{

}

