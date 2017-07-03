#version 330 core

in vec3 vertex_modelspace;

uniform int Seed;

out float color;

#include <classicnoise3D.glsl>

const float PI = 3.14159265359;

const vec3 blue_sea = vec3(0, 0.22, 0.48);
const vec3 green_grass = vec3(0, 0.4, 0);
const vec3 green_forest = vec3(0.04, 0.25, 0.04);
const vec3 brown_mountain = vec3(0.26, 0.14, 0.03);

const float sea_offset = 0.55;
const float mountain_offset = 0.7;

float get_colour(vec3 vertex) {
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
	} else  {
		height = (pt - sea_offset) / (1 - sea_offset);
	} 

	return height;
}

void main() {
	float height = get_colour(vertex_modelspace);
	color = height;
}