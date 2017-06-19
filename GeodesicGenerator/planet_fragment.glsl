#version 330 core

in vec3 v_colour;
in vec3 normal_worldspace;
in vec3 camera_direction;
in vec3 vertex_worldspace;
in vec3 vertex_scatter;

uniform vec3 light_direction;
uniform vec3 light_colour;
uniform float light_power;
uniform mat4 M;
uniform bool isAtmos;

out vec3 color;

const float MAX = 10000.0;

const float K_R = 0.166;
const float K_M = 0.0025;
const float E = 14.3; 						// light intensity
const vec3  C_R = vec3(0.3, 0.7, 1.0); 	// 1 / wavelength ^ 4
const float G_M = -0.85;

const float PI = 3.14159265359;

const float FNUM_STEPS = 10.0;
const int NUM_STEPS = 10;

const float R = 1.05;
const float R_INNER = 0.99;
const float SCALE_HEIGHT = 1.0 / (0.25 * (R - R_INNER));
const float SCALE_LENGTH = 1.0 / (R - R_INNER);

vec3 origin_worldspace = (M*vec4(0, 0, 0, 1)).xyz;;


vec3 sphere_int(vec3 ray_ori, vec3 ray_dir, vec3 sphere_ori, float rad) {
	//float a = dot(ray_dir, ray_dir); Assuming ray_dir is normalised a = 1
	float b = 2 * dot(ray_dir, ray_ori - sphere_ori);
	float c = dot(ray_ori - sphere_ori, ray_ori - sphere_ori) - rad * rad;

	float det = (b * b) - (4 * c);
	if(det < 0) {
		return vec3(MAX, MAX, MAX);
	}
	det = sqrt(det);
	float t1 = 0.5 * (-b + det);
	float t2 = 0.5 * (-b - det);
	return ray_ori + ray_dir * max(t1, t2);
}

float density(vec3 p) {
	return exp(-(length(p - origin_worldspace) - R_INNER) * SCALE_HEIGHT);
}

float out_scatter(vec3 p1, vec3 p2) {
	vec3 step = (p2 - p1) / FNUM_STEPS;
	vec3 p = p1 + step * 0.5;

	float sum = 0.0;
	for(int i = 0; i < NUM_STEPS; i++) {
		sum += density(p);
		p += step;
	}
	sum *= length(step) * SCALE_LENGTH;

	return sum;
}

void main() {
	vec3 light_dir_norm = normalize(light_direction);
	float cos_theta = clamp(dot(normal_worldspace, light_dir_norm), 0, 1);

	vec3 reflection_direction = reflect(-light_dir_norm, normal_worldspace);

	float cos_alpha = clamp(dot(reflection_direction, camera_direction), 0, 1);

	float n = out_scatter(vertex_worldspace, sphere_int(vertex_worldspace, camera_direction, origin_worldspace, R));
		+ out_scatter(vertex_worldspace, sphere_int(vertex_worldspace, light_direction, origin_worldspace, R));

	if(isAtmos) {
		color = vertex_scatter;
	} else {
		color = (v_colour * light_colour * light_power * cos_theta)
			+ v_colour * light_colour * light_power * pow(cos_alpha, 5) * cos_theta * 0.3
			+ v_colour * light_colour * 0.1;
		color = color * exp(-n * PI * 4 * (K_R * C_R + K_M)) + vertex_scatter;
	}
}