#version 330 core

in vec3 vertex_position;
in vec3 vertex_normal;
in vec3 vertex_colour;

uniform mat4 MVP;
uniform mat4 M;
uniform vec3 camera_position;
uniform vec3 light_direction;
uniform vec3 light_colour;

out vec3 v_colour;
out vec3 vertex_worldspace;
out vec3 normal_worldspace;
out vec3 camera_direction;
out vec3 scatter_colour;

const float K_R = 0.166;
const float K_M = 0.0025;
const float E = 14.3; 						// light intensity
const vec3  C_R = vec3( 0.3, 0.7, 1.0 ); 	// 1 / wavelength ^ 4
const float G_M = -0.85;

const float PI = 3.14159265359;

const float FNUM_STEPS = 10.0;
const int NUM_STEPS = 10;

const float R = 1.5;
const float R_INNER = 1.0;
const float SCALE_HEIGHT = 1.0/(0.25 * (R - R_INNER));
const float SCALE_LENGTH = 1.0/(R - R_INNER);

// www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
vec3 sphere_intersect(vec3 ray_origin, vec3 ray_direction, vec3 sphere_origin, float radius){
	vec3 L = sphere_origin - ray_origin;
	float t_ca = dot(L, ray_direction);
	if(t_ca < 0) return vec3(0,0,0);

	float dd = dot(L, L) - t_ca * t_ca;
	float t_hc = sqrt(radius*radius - dd);
	return ray_origin + ray_direction * (t_ca - t_hc);
}

float phase_reyleigh( float cc ) {
	return 0.75 * ( 1.0 + cc );
}

float phase_mie( float g, float c, float cc ) {
	float gg = g * g;
	
	float a = ( 1.0 - gg ) * ( 1.0 + cc );

	float b = 1.0 + gg - 2.0 * g * c;
	b *= sqrt( b );
	b *= 2.0 + gg;	
	
	return 1.5 * a / b;
}

float density(vec3 p){
	return exp(-(length(p) - R_INNER) * SCALE_HEIGHT);
}

float out_scatter(vec3 p1, vec3 p2){
	vec3 step = (p2 - p1) / FNUM_STEPS;
	vec3 v = p1 + step * 0.5;

	float sum = 0.0;
	for(int i = 0; i < NUM_STEPS; i++){
		sum += density(v);
		v += step;
	}
	return 4.0 * PI * sum;
}

vec3 in_scatter(vec3 p1, vec3 p2, vec3 sun_direction){
	vec3 step = (p2 -p1) / FNUM_STEPS;
	vec3 p = p1 + step * 0.5;

	vec3 sphere_origin = (M*vec4(0,0,0,1)).xyz;
	float sphere_radius = length(sphere_origin - (M * vec4(vertex_position, 1)).xyz);

	vec3 sum = vec3(0.0);
	for(int i = 0; i <  NUM_STEPS; i++){
		vec3 pc = sphere_intersect(p+sun_direction, -sun_direction, sphere_origin, sphere_radius);
		
		float n = out_scatter(pc, p) + out_scatter(p, p2); 

		sum += density(p) * exp(-n * (K_R * C_R + K_M));
		p += step;
	}

	float cos = dot(normalize(p2 -p1), normalize(sun_direction));
	float cc = cos * cos;

	return sum * ( K_R * C_R * phase_reyleigh( cc ) + K_M * phase_mie( G_M, cos, cc ) );
}

void main(){
	gl_Position = MVP * vec4(vertex_position, 1);

	vertex_worldspace = (M * vec4(vertex_position, 1)).xyz;
    
    normal_worldspace = normalize(M * vec4(vertex_normal, 0)).xyz;
    
    camera_direction = normalize(camera_position - vertex_worldspace);
    
    //light_direction = light_position - (M * vec4(vertex_position, 1)).xyz;
    
	scatter_colour = light_colour * in_scatter(vertex_worldspace, sphere_intersect(camera_position, camera_direction, (M*vec4(0,0,0,1)).xyz, length((M*vec4(0,0,0,1)).xyz- vertex_worldspace)), light_direction);
	v_colour = vertex_colour;
}