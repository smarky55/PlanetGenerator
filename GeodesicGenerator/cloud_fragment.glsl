#version 330 core

in vec3 vertex_modelspace;

uniform int Seed;

out vec4 color;

#include <classicnoise3D.glsl>
#include <constants.glsl>

const float cloud_offset = 0.2;

vec4 get_cloud(vec3 vertex) {
	int seed = Seed + 20;
	float mag = 0;
	float pt = 0;
	for(int i = 0; i < 6; i++) {
		float factor = pow(2, -(i+2));
		pt += noise(vertex / factor, seed++) * factor;
		mag += factor;
	}
	pt /= mag;
	pt = clamp(pt, cloud_offset, 1);
	vec4 colour = vec4(pt);
	colour.a = (pt - cloud_offset) / (1 - cloud_offset);
	return colour;
}

void main() {
	int seed = Seed+20;
	color = get_cloud(vertex_modelspace);
	//color = vec4(pt);
	//color.a = color.a * 2 - 1;
}