#version 330 core

in vec3 vertex_modelspace;

uniform int Seed;

out vec3 color;

#include <classicnoise3D.glsl>
#include <constants.glsl>

void main() {
	int seed = Seed+20;
	float pt = noise(vertex_modelspace, seed++);
	pt = clamp(pt, 0, 1);
	color = vec3(pt);
}