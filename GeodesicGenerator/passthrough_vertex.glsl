#version 330 core

in vec3 vertex_position;

uniform samplerCube normal_map;

out vec3 normal_modelspace;

void main() {
	gl_Position = vec4(vertex_position, 1);

	normal_modelspace = (texture(normal_map, vertex_position).xyz *2) - 1;

}