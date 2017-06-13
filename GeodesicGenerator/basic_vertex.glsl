#version 330 core

in vec3 vertex_position;
in vec3 vertex_colour;

uniform mat4 MVP;

out vec3 v_colour;

void main(){
	gl_Position = MVP * vec4(vertex_position, 1);
    gl_PointSize = 10.0;
	v_colour = vertex_colour;
}