#version 330 core

in vec3 v_colour;
in vec3 normal_worldspace;
in vec3 camera_direction;

uniform vec3 light_direction;
uniform vec3 light_colour;
uniform float light_power;

out vec3 color;

void main() {
	vec3 light_dir_norm = normalize(light_direction);
	float cos_theta = clamp(dot(normal_worldspace, light_dir_norm), 0, 1);

	vec3 reflection_direction = reflect(-light_dir_norm, normal_worldspace);

	float cos_alpha = clamp(dot(reflection_direction, camera_direction), 0, 1);

	color = (v_colour * light_colour * light_power * cos_theta)
		+ v_colour * light_colour * light_power * pow(cos_alpha, 5) * 0.3
		+ v_colour * light_colour * 0.1;
}