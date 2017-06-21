#version 330 core

in vec3 vertex_position;
in vec3 vertex_normal;

uniform mat4 MVP;
uniform mat4 M;
uniform vec3 camera_position;
uniform vec3 light_direction;
uniform bool isAtmos;

out vec3 vertex_worldspace;
out vec3 vertex_modelspace;
out vec3 vertex_scatter;
out vec3 normal_worldspace;
out vec3 camera_direction;

vec3 origin_worldspace = (M*vec4(0, 0, 0, 1)).xyz;

#include <scattering.glsl>

void main() {
	gl_Position = MVP * vec4(vertex_position, 1);

	normal_worldspace = normalize(M * vec4(vertex_normal, 0)).xyz;

	vertex_modelspace = vertex_position;
	vertex_worldspace = (M * vec4(vertex_position, 1)).xyz;

	camera_direction = normalize(camera_position - vertex_worldspace);

	vertex_scatter = scatter(vertex_worldspace, camera_direction, origin_worldspace, R);
}