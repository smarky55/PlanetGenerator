#version 330 core

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_colour;

uniform mat4 MVP;
uniform mat4 M;
uniform vec3 camera_position;

out vec3 v_colour;
out vec3 normal_worldspace;
out vec3 light_direction;
out vec3 camera_direction;

void main(){
	gl_Position = MVP * vec4(vertex_position, 1);
    
    normal_worldspace = normalize(M * vec4(vertex_normal, 0)).xyz;
    
    camera_direction = normalize(camera_position - (M * vec4(vertex_position, 1)).xyz);
    
    //light_direction = light_position - (M * vec4(vertex_position, 1)).xyz;
    
	v_colour = vertex_colour;
}