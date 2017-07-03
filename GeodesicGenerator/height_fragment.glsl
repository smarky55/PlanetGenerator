#version 330 core

in vec3 vertex_modelspace;

uniform int Seed;

out float color;

#include <classicnoise3D.glsl>

#include <constants.glsl>

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
	color = height;
}