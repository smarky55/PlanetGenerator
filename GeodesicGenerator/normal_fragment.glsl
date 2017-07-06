#version 400 core

in vec3 vertex_modelspace;

uniform int Seed;

out vec3 color;

#include <classicnoise3D.glsl>

#include <constants.glsl>

const float sinx = sin(grad_step);
const float cosx = cos(grad_step);

vec3 cartesian_to_polar(vec3 cart) {
	vec3 polar;
	float xx = cart.x*cart.x;
	float zz = cart.z*cart.z;
	float yy = cart.y*cart.y;
	polar.x = sqrt(xx+yy+zz); //r
	polar.y = atan(cart.z , cart.x); // theta
	polar.z = acos(cart.y / polar.x); // phi

	return polar;
}

vec3 polar_to_cartesian(vec3 polar) {
	vec3 cart;
	float sinphi = sin(polar.z);
	cart.x = polar.x * cos(polar.y) * sinphi; // r * cos(theta) * sin(phi)
	cart.z = polar.x * sin(polar.y) * sinphi; // r * sin(theta) * sin(phi)
	cart.y = polar.x * cos(polar.z); // r * cos(phi)

	return cart;
}

vec4 rot_quart(vec3 axis) {
	vec4 q = vec4(0);
	q.x = axis.x * sinx;
	q.y = axis.y * sinx;
	q.z = axis.z * sinx;
	q.w = cosx;
	return q;
}

mat3 rotate(vec3 axis) {
	vec4 q = rot_quart(normalize(axis));
	mat3 rot;
	rot[0] = vec3(1 - 2 * (q.y*q.y + q.z*q.z), 
				   2 * (q.x*q.y - q.z*q.w), 
				   2 * (q.x*q.z + q.y*q.w));
	rot[1] = vec3(2 * (q.x*q.y + q.z*q.w), 
				   1 - 2 * (q.x*q.x + q.z*q.z),
				   2 * (q.y*q.z - q.x*q.w));
	rot[2] = vec3(2 * (q.x*q.z - q.y*q.w),
				   2 * (q.y*q.z + q.x*q.w), 
				   1 - 2 * (q.x*q.x + q.y*q.y));
	return rot;
}

mat3 rot90(vec3 axis) {
	vec4 q = vec4(axis, 0);
	mat3 rot;
	rot[0] = vec3(1 - 2 * q.y*q.y - 2 * q.z*q.z, 2 * q.x*q.y - 2 * q.z*q.w, 2 * q.x*q.z + 2 * q.y*q.w);
	rot[1] = vec3(2 * q.x*q.y + 2 * q.z*q.w, 1 - 2 * q.x*q.x - 2 * q.z*q.z, 2 * q.y*q.z - 2 * q.x*q.w);
	rot[2] = vec3(2 * q.x*q.z - 2 * q.y*q.w, 2 * q.y*q.z + 2 * q.x*q.w, 1 - 2 * q.x*q.x - 2 * q.y*q.y);
	return rot;
}

mat3 rot90y() {
	mat3 rot;
	rot[0] = vec3(0, 0, 1);
	rot[1] = vec3(0, 1, 0);
	rot[2] = vec3(-1, 0, 0);
	return rot;
}


float get_height(vec3 vertex) {
	int seed = Seed;
	float pt = (noise(vertex / 0.75, seed++) * 0.4
				+ noise(vertex / 0.5, seed++) * 0.5
				+ noise(vertex / 0.25, seed++) * 0.25
				+ noise(vertex / 0.1, seed++) * 0.1
				+ noise(vertex / 0.05, seed++) * 0.075) / 1.325;

	float latitude = PI*0.5 - acos(dot(vertex, vec3(0, 1, 0)));
	float temp = ((noise(vertex / 0.3, seed++) + 2) / 3) * cos(latitude) * 50 - 20;

	float height;
	
	height = clamp((pt - sea_offset) / (1 - sea_offset), 0, 1);

	return pow(height, 1.8);
}

void main() {
	float height = get_height(vertex_modelspace);


	vec3 vert_polar = cartesian_to_polar(vertex_modelspace);
	
	//vec3 vert_dtheta = polar_to_cartesian(vert_polar + vec3(0, step, 0));
	vec3 vert_dtheta = vec3(rotate(vec3(0, 1, 0)) * vertex_modelspace);
	float height_dtheta = get_height(vert_dtheta);

	//vec3 vert_dphi = polar_to_cartesian(vert_polar + vec3(0, 0, step));
	vec3 vert_dphi = vec3(rotate(rot90y() * normalize(vec3(vertex_modelspace.x, 0, vertex_modelspace.z)))
						   * vertex_modelspace);
	float height_dphi = get_height(vert_dphi);

	vec3 grad_theta = vert_dtheta * (1 + height_dtheta * height_scale) 
		- vertex_modelspace *(1 + height * height_scale);
	vec3 grad_phi = vert_dphi * (1 + height_dphi * height_scale)
		- vertex_modelspace *(1 + height * height_scale);

	color = normalize(cross(grad_theta, grad_phi));
}