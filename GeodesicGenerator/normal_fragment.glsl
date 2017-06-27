#version 330 core

in vec3 vertex_modelspace;

uniform int Seed;

out vec3 color;

#include <classicnoise3D.glsl>

const float PI = 3.14159265359;

const vec3 blue_sea = vec3(0, 0.22, 0.48);
const vec3 green_grass = vec3(0, 0.4, 0);
const vec3 green_forest = vec3(0.04, 0.25, 0.04);
const vec3 brown_mountain = vec3(0.26, 0.14, 0.03);

const float sea_offset = 0.55;
const float mountain_offset = 0.7;

vec3 cartesian_to_polar(vec3 cart) {
	vec3 polar;
	float xx = cart.x*cart.x;
	float yy = cart.z*cart.z;
	polar.x = sqrt(xx + yy + cart.y*cart.y); //r
	polar.y = atan(cart.z , cart.x); // theta
	polar.z = acos(cart.y / polar.x); // phi

	return polar;
}

vec3 polar_to_cartesian(vec3 polar) {
	vec3 cart;
	float sinphi = sin(polar.z);
	cart.x = polar.x * cos(polar.y) * sinphi;
	cart.z = polar.x * sin(polar.y) * sinphi;
	cart.y = polar.x * cos(polar.z);

	return cart;
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
	if(pt < sea_offset) {
		height = 0;
	} else {
		height = (pt - sea_offset) / (1 - sea_offset);
	}
	if(temp < 0) {
		height *= 1 + temp/20;
	}

	return height;
}

const float height_scale = 1.0;

void main() {
	float height = get_height(vertex_modelspace);

	vec3 vert_polar = cartesian_to_polar(vertex_modelspace);
	
	vec3 vert_dtheta = polar_to_cartesian(vert_polar + vec3(0, 0.0001, 0));
	float height_dtheta = get_height(vert_dtheta);

	vec3 vert_dphi = polar_to_cartesian(vert_polar + vec3(0, 0, 0.0001));
	float height_dphi = get_height(vert_dphi);

	vec3 grad_theta = vert_dtheta * (1 + height_dtheta * height_scale) 
		- vertex_modelspace * (1 + height * height_scale);
	vec3 grad_phi = vert_dphi * (1 + height_dphi * height_scale) 
		- vertex_modelspace * (1 + height * height_scale);
	
	color =  (normalize(cross(grad_phi, grad_theta)) + 1) * 0.5;
}