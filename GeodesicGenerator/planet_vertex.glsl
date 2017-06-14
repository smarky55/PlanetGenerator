#version 330 core

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_colour;

uniform mat4 MVP;
uniform mat4 M;
uniform vec3 camera_position;
uniform vec3 light_direction;

out vec3 v_colour;
out vec3 vertex_worldspace;
out vec3 vertex_scatter;
out vec3 normal_worldspace;
out vec3 camera_direction;
out float debug;
out vec3 vdebug;

const float MAX = 10000.0;

const float K_R = 0.166;
const float K_M = 0.0025;
const float E = 14.3; 						// light intensity
const vec3  C_R = vec3(0.3, 0.7, 1.0); 	// 1 / wavelength ^ 4
const float G_M = -0.85;

const float PI = 3.14159265359;

const float FNUM_STEPS = 10.0;
const int NUM_STEPS = 10;

const float R = 1.1;
const float R_INNER = 0.99;
const float SCALE_HEIGHT = 1.0 / (0.25 * (R - R_INNER));
const float SCALE_LENGTH = 1.0 / (R - R_INNER);

vec3 sphere_int(vec3 ray_ori, vec3 ray_dir, vec3 sphere_ori, float rad) {
	//float a = dot(ray_dir, ray_dir); Assuming ray_dir is normalised a = 1
	float b = 2 * dot(ray_dir, ray_ori - sphere_ori);
	float c = dot(ray_ori - sphere_ori, ray_ori - sphere_ori) - rad * rad;

	float det = (b * b) - (4 * c);
	debug = det;
	if(det < 0) {
		return vec3(MAX, MAX, MAX);
	}
	det = sqrt(det);
	float t1 = 0.5 * (-b + det);
	float t2 = 0.5 * (-b - det);
	return ray_ori + ray_dir * max(t1, t2);
}

float phase_rayleigh(float cc) {
	return 0.75 * (1.0 + cc);
}

float phase_mie(float g, float c, float cc) {
	float gg = g * g;

	float a = (1.0 - gg) * (1.0 + cc);

	float b = 1.0 + gg - 2.0 * g * c;
	b *= sqrt(b);
	b *= 2.0 + gg;

	return 1.5 * a / b;
}

float density(vec3 p) {
	return exp(-(length(p) - R_INNER) * SCALE_HEIGHT);
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

vec3 scatter(vec3 p1, vec3 cam_dir, vec3 s_ori, float rad) {
	vec3 p2 = sphere_int(p1, cam_dir, s_ori, rad);
	vec3 step = (p2 - p1) / FNUM_STEPS;
	vec3 p = p1 + step * 0.5;
	vdebug = p2;
	vec3 sum = vec3(0);
	//float sum = 0;
	for(int i = 0; i < NUM_STEPS; i++) {
		vec3 pc = sphere_int(p, light_direction, vec3(0), R);

		float n = (out_scatter(pc, p) + out_scatter(p, p2)) * (PI * 4.0);

		sum += density(p) * exp(-n * ( K_R * C_R + K_M));
		p += step;
	}
	sum *= length(step) * SCALE_LENGTH;

	float c = dot(cam_dir, normalize(light_direction));
	float cc = c*c;
	debug = phase_mie(G_M, c, cc);
	return (K_R * C_R * phase_rayleigh(cc) + K_M * phase_mie(G_M, c, cc)) * sum * 5;
}

void main() {
	gl_Position = MVP * vec4(vertex_position, 1);

	normal_worldspace = normalize(M * vec4(vertex_normal, 0)).xyz;

	vertex_worldspace = (M * vec4(vertex_position, 1)).xyz;

	camera_direction = normalize(camera_position - vertex_worldspace);
	//v_colour = scatter(vertex_worldspace, camera_direction, vec3(0), R);
	//float n = out_scatter(vertex_worldspace, sphere_int(vertex_worldspace, camera_direction, vec3(0), R))
	//	+ out_scatter(vertex_worldspace, sphere_int(vertex_worldspace, light_direction, vec3(0), R));
	v_colour = vertex_colour;
	vertex_scatter = scatter(vertex_worldspace, camera_direction, vec3(0), R);
}