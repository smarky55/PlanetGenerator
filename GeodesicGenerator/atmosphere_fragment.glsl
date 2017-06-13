#version 330 core

in vec3 v_colour;
in vec3 vertex_worldspace;
in vec3 normal_worldspace;
in vec3 camera_direction;
in vec3 scatter_colour;

uniform vec3 light_direction;
uniform vec3 light_colour;
uniform float light_power;

out vec3 color;

const float PI = 3.14159265359;

const float FNUM_STEPS = 10.0;
const int NUM_STEPS = 10;

const float R = 1.5;
const float R_INNER = 1.0;
const float SCALE_HEIGHT = 1.0/(0.25 * (R - R_INNER));
const float SCALE_LENGTH = 1.0/(R - R_INNER);

vec3 sphere_intersect(vec3 ray_origin, vec3 ray_direction, vec3 sphere_origin, float radius){
	vec3 L = sphere_origin - ray_origin;
	float t_ca = dot(L, ray_direction);
	if(t_ca < 0) return vec3(0,0,0);

	float dd = dot(L, L) - t_ca * t_ca;
	float t_hc = sqrt(radius*radius - dd);
	return ray_origin + ray_direction * (t_ca - t_hc);
}

float out_scatter(vec3 p1, vec3 p2){
	vec3 step = (p2 - p1) / FNUM_STEPS;
	vec3 v = p1 + step * 0.5;

	float sum = 0.0;
	for(int i = 0; i < NUM_STEPS; i++){
		sum += exp(-(length(v) - R_INNER) * SCALE_HEIGHT);
		v += step;
	}
	return 4.0 * PI * sum;
}

void main(){
    vec3 light_dir_norm = normalize(light_direction);
    float cos_theta = clamp(dot(normal_worldspace, light_dir_norm), 0, 1);
    
    vec3 reflection_direction = reflect(-light_dir_norm, normal_worldspace);
    
    float cos_alpha = clamp(dot(reflection_direction, camera_direction), 0, 1);
    
	color = (v_colour * light_colour * light_power * cos_theta)
            + v_colour * light_colour * light_power * pow(cos_alpha, 5) * 0.3
            + v_colour * light_colour * 0.1;
	color = scatter_colour;
}