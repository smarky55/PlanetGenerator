#version 330 core

// _State_
// 0: Planet surface
// 1: Atmosphere
// 2: Clouds

in vec3 camera_direction;
in vec3 vertex_worldspace;
in vec3 vertex_modelspace;
in vec3 vertex_scatter;

uniform vec3 light_direction;
uniform vec3 light_colour;
uniform float light_power;
uniform mat4 M;
uniform int state;
uniform int Seed;

uniform samplerCube cube_texture;
uniform samplerCube normal_map;
uniform samplerCube cloud_map;

out vec4 color;

vec3 origin_worldspace = (M*vec4(0, 0, 0, 1)).xyz;

#include <constants.glsl>
#include <scattering.glsl>


vec4 rot_quart(vec3 axis, float angle) {
	vec4 q = vec4(0);
	float sinx = sin(angle);
	q.x = axis.x * sinx;
	q.y = axis.y * sinx;
	q.z = axis.z * sinx;
	q.w = cos(angle);
	return q;
}

mat3 rotate(vec3 axis, float angle) {
	vec4 q = rot_quart(normalize(axis), angle);
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


void main() {
	// Bypass unneeded calculation if atmosphere
	if(state == 1) {
		color = vec4(vertex_scatter, 1);
	} else {
		vec4 f_colour;
		vec3 normal_worldspace;

		// switch texture lookup based on state TODO: bind cloud texture instead of planet
		if(state == 0) {
			f_colour = texture(cube_texture, vertex_modelspace);
			normal_worldspace = normalize(M * vec4(texture(normal_map, vertex_modelspace).xyz, 0)).xyz;
		} else if(state == 2) {
			float angle = acos(normalize(vertex_modelspace).y);
			vec3 cloud_vertex = rotate(vec3(0, 1, 0), 0.3 * sin(2 * angle)) * vertex_modelspace;
			f_colour = texture(cloud_map, cloud_vertex);
			normal_worldspace = normalize(vertex_worldspace);
		} else {
			f_colour = vec4(0);
			normal_worldspace = vec3(0);
		}

		vec3 light_dir_norm = normalize(light_direction);
		float cos_theta = clamp(dot(normal_worldspace, light_dir_norm), 0, 1);

		vec3 reflection_direction = reflect(-light_dir_norm, normal_worldspace);

		// Switch whether texture includes specular coeff.
		float cos_alpha;
		if(state == 0) {
			cos_alpha = clamp(f_colour.a * dot(reflection_direction, camera_direction), 0, 1);
		} else {
			cos_alpha = clamp(0.1 * dot(reflection_direction, camera_direction), 0, 1);
		}

		float n = out_scatter(vertex_worldspace, sphere_int(vertex_worldspace, camera_direction, origin_worldspace, R))
				+ out_scatter(vertex_worldspace, sphere_int(vertex_worldspace, light_direction, origin_worldspace, R));

		vec3 t_color;
		t_color = (f_colour.rgb * light_colour * light_power * cos_theta)
			+ f_colour.rgb * light_colour * light_power * pow(cos_alpha, 2) * cos_theta
			+ f_colour.rgb * light_colour * 0.1;
		t_color = t_color * exp(-n * PI * 4 * (K_R * C_R + K_M)) + vertex_scatter;
		if(state == 2) {
			color = vec4(t_color, f_colour.a);
		} else {
			color = vec4(t_color, 1);
		}
	}
}