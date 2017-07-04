#version 330 core

//in vec3 normal_worldspace;
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

uniform samplerCube cube_texture;
uniform samplerCube normal_map;
uniform samplerCube cloud_map;

out vec3 color;

vec3 origin_worldspace = (M*vec4(0, 0, 0, 1)).xyz;

#include <constants.glsl>
#include <scattering.glsl>


void main() {
	vec4 f_colour, cloud_colour;
	cloud_colour = texture(cloud_map, vertex_modelspace);
	f_colour = texture(cube_texture, vertex_modelspace);
	f_colour = mix(f_colour, vec4(cloud_colour.rgb, 0.1), cloud_colour.a);

	vec3 normal_worldspace = (M * vec4(texture(normal_map, vertex_modelspace).xyz, 0)).xyz;
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