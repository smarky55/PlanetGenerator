#version 330 core

in vec3 vertex_position;

uniform mat4 VP;

out vec3 vertex_modelspace;

void main() {
	gl_Position = VP * vec4(vertex_position, 1);

	vertex_modelspace = vertex_position;
}