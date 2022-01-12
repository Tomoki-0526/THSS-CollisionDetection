#pragma once

#include "const.h"

class Spheres
{
public:
	float x[g_size], y[g_size], z[g_size];
	float vx[g_size], vy[g_size], vz[g_size];
	float r[g_size], m[g_size];
	float red[g_size], green[g_size], blue[g_size];
	float next_x[g_size], next_y[g_size], next_z[g_size];
	float next_vx[g_size], next_vy[g_size], next_vz[g_size];

	float ambient_r[g_size], ambient_g[g_size], ambient_b[g_size];
	float diffuse_r[g_size], diffuse_g[g_size], diffuse_b[g_size];
	float specular_r[g_size], specular_g[g_size], specular_b[g_size];
	float shininess[g_size];
};

