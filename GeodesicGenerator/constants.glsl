
// Numerical Constants
const float MAX = 10000.0;
const float PI = 3.14159265359;

// Scattering parameters
const float K_R = 0.166;
const float K_M = 0.0025;
const float E = 14.3; 						// light intensity
const vec3  C_R = vec3(0.3, 0.7, 1.0); 	// 1 / wavelength ^ 4
const float G_M = -0.85;

// Scattering integration steps
const float FNUM_STEPS = 10.0;
const int NUM_STEPS = 10;

// Atmosphere properties
const float R = 1.05;
const float R_INNER = 0.99;
const float SCALE_HEIGHT = 1.0 / (0.25 * (R - R_INNER));
const float SCALE_LENGTH = 1.0 / (R - R_INNER);

// Colours
const vec3 blue_sea = vec3(0, 0.22, 0.48);
const vec3 green_grass = vec3(0, 0.4, 0);
const vec3 green_forest = vec3(0.04, 0.25, 0.04);
const vec3 brown_mountain = vec3(0.26, 0.14, 0.03);

// Terrain levels
const float sea_offset = 0.55;
const float mountain_offset = 0.7;

// Normal calculation
const float height_scale = 0.1;
const float grad_step = 0.005;