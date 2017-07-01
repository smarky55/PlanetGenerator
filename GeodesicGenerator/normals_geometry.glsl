#version 330 core

layout(triangles) in;

layout(line_strip, max_vertices = 6) out;

in vec3 normal_modelspace[];

uniform float normal_length;
uniform mat4 MVP;

out vec3 v_colour;

void main() {
	for(int i = 0; i < gl_in.length(); i++) {
		vec3 pos = gl_in[i].gl_Position.xyz;
		vec3 norm = normal_modelspace[i];

		gl_Position = MVP * vec4(pos, 1);
		v_colour = vec3(0, 0.8, 0);
		EmitVertex();

		gl_Position = MVP * vec4(pos + norm * normal_length, 1);
		v_colour = vec3(0, 0.8, 0);
		EmitVertex();

		EndPrimitive();
	}
}