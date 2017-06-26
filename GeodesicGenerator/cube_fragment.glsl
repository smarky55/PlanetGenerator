#version 330 core

in vec3 vertex_worldspace;

uniform int Seed;

out vec4 color;

#include <classicnoise3D.glsl>

const float PI = 3.14159265359;

const vec3 blue_sea = vec3(0, 0.22, 0.48);
const vec3 green_grass = vec3(0, 0.4, 0);
const vec3 green_forest = vec3(0.04, 0.25, 0.04);
const vec3 brown_mountain = vec3(0.26, 0.14, 0.03);

const float sea_offset = 0.55;
const float mountain_offset = 0.7;

vec4 get_colour(vec3 vertex) {
	int seed = Seed;
	float pt = (noise(vertex / 0.75, seed++) * 0.4
				+ noise(vertex / 0.5, seed++) * 0.5
				+ noise(vertex / 0.25, seed++) * 0.25
				+ noise(vertex / 0.1, seed++) * 0.1
				+ noise(vertex / 0.05, seed++) * 0.075) / 1.325;

	float latitude = PI*0.5 - acos(dot(vertex, vec3(0, 1, 0)));
	float temp = ((noise(vertex / 0.3, seed++) + 2) / 3) * cos(latitude) * 50 - 20;

	vec4 colour;
	if(pt < sea_offset) {
		colour = vec4(blue_sea * ((pt + 0.1) / (sea_offset + 0.1)), (noise(vertex / 0.01, seed++) + 3)*0.25);
	} else if(pt < mountain_offset) {
		vec3 green = mix(green_grass, green_forest, noise(vertex / 0.15, seed++));
		colour = mix(vec4(green * pt / mountain_offset, 0.1),
					 vec4(brown_mountain, 0.1),
					 pow((pt - sea_offset) / (mountain_offset - sea_offset), 2));
	} else {
		colour = mix(vec4(brown_mountain, 0.1), vec4(1, 1, 1, 0.1),
					 pow((pt - mountain_offset) / (1 - mountain_offset), 1) * noise(vertex / 0.05, seed++));
	}

	if(temp < 0) {
		colour = mix(colour, vec4(1, 1, 1, 0.2), abs(temp / 20) + 0.5);
	}

	return colour;
}

void main() {
	vec4 f_colour = get_colour(vertex_worldspace);
	color = f_colour;
}