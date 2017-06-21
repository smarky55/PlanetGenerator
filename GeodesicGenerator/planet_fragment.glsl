#version 330 core

in vec3 normal_worldspace;
in vec3 camera_direction;
in vec3 vertex_worldspace;
in vec3 vertex_modelspace;
in vec3 vertex_scatter;

uniform vec3 light_direction;
uniform vec3 light_colour;
uniform float light_power;
uniform mat4 M;
uniform bool isAtmos;
uniform int Seed;

out vec3 color;

vec3 origin_worldspace = (M*vec4(0, 0, 0, 1)).xyz;

#include <scattering.glsl>

#include <classicnoise3d.glsl>

const vec3 blue_sea = vec3(0, 0.22, 0.48);
const vec3 green_grass = vec3(0, 0.4, 0);
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
	float temp = ((noise(vertex/0.3, seed++) + 2)/3) * cos(latitude) * 50 - 20;

	vec4 colour;
	if(pt < sea_offset) {
		colour = vec4(blue_sea * ((pt + 0.1) / (sea_offset + 0.1)), (noise(vertex / 0.01, seed++)+3)*0.25);
	} else if(pt < mountain_offset) {
		colour = mix(vec4(green_grass * pt / mountain_offset, 0.1),
					 vec4(brown_mountain, 0.1),
					 pow((pt - sea_offset) / (mountain_offset - sea_offset), 4));
	} else {
		colour = mix(vec4(brown_mountain, 0.1), vec4(1, 1, 1, 0.1),
					 pow((pt - mountain_offset)/(1-mountain_offset), 0.9) * noise(vertex / 0.05, seed++));
	}

	if(temp < 0) {
		colour = mix(colour, vec4(1, 1, 1, 0.2), abs(temp / 20) + 0.5);
	}

	return colour;
}

void main() {
	vec4 f_colour = get_colour(vertex_modelspace);

	vec3 light_dir_norm = normalize(light_direction);
	float cos_theta = clamp(dot(normal_worldspace, light_dir_norm), 0, 1);

	vec3 reflection_direction = reflect(-light_dir_norm, normal_worldspace);

	float cos_alpha = clamp(f_colour.a * dot(reflection_direction, camera_direction), 0, 1);

	float n = out_scatter(vertex_worldspace, sphere_int(vertex_worldspace, camera_direction, origin_worldspace, R));
		+ out_scatter(vertex_worldspace, sphere_int(vertex_worldspace, light_direction, origin_worldspace, R));

	if(isAtmos) {
		color = vertex_scatter;
	} else {
		color = (f_colour.rgb * light_colour * light_power * cos_theta)
			+ f_colour.rgb * light_colour * light_power * pow(cos_alpha, 2) * cos_theta
			+ f_colour.rgb * light_colour * 0.1;
		color = color * exp(-n * PI * 4 * (K_R * C_R + K_M)) + vertex_scatter;
	}
}