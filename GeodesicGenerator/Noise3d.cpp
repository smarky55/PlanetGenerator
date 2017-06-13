#include "Noise3d.h"



Noise3d::Noise3d(unsigned long long seed, double wavelength) {
	Seed = seed;
	Wavelength = wavelength;
}


Noise3d::~Noise3d() {
}

double Noise3d::fade(double t) {
	// 6t^5 - 15t^4 + 10t^3
	return (t * t * t * (t * (t * 6 - 15) + 10));
}

double Noise3d::lerp(double a, double b, double x) {
	return a + x*(b - a);
}

double Noise3d::grad(size_t hash, double x, double y, double z) {
	switch(hash & 7) {
	case 0:
		return x + y + z;
	case 1:
		return x + y - z;
	case 2:
		return x - y + x;
	case 3:
		return x - y - z;
	case 4:
		return -x + y + z;
	case 5:
		return -x + y - z;
	case 6:
		return -x - y + z;
	case 7:
		return -x - y - z;
	default:
		return 0;
	}
}

size_t Noise3d::getHash(long long x, long long y, long long z) {
	long long buffer[] = {x,y,z};
	size_t hash = XXH64(buffer, sizeof(buffer), Seed);
	return hash;
}

size_t Noise3d::getHash(long long x, long long y, long long z, size_t seed) {
	long long buffer[] = {x,y,z};
	size_t hash = XXH64(buffer, sizeof(buffer), seed);
	return hash;
}

double Noise3d::getPoint(double x, double y, double z) {
	x /= Wavelength;
	y /= Wavelength;
	z /= Wavelength;

	long long xi = (long long)x;
	long long yi = (long long)y;
	long long zi = (long long)z;

	double xf = abs(x - xi);
	if(x < 0) {
		xf = 1 - xf;
		xi--;
	}
	double yf = abs(y - yi);
	if(y < 0) {
		yf = 1 - yf;
		yi--;
	}
	double zf = abs(z - zi);
	if(z < 0) {
		zf = 1 - zf;
		zi--;
	}

	double u = fade(xf);
	double v = fade(yf);
	double w = fade(zf);

	size_t aaa, aab, aba, abb, baa, bab, bba, bbb;
	aaa = getHash(xi, yi, zi);
	aab = getHash(xi, yi, zi + 1);
	aba = getHash(xi, yi + 1, zi);
	abb = getHash(xi, yi + 1, zi + 1);
	baa = getHash(xi + 1, yi, zi);
	bab = getHash(xi + 1, yi, zi + 1);
	bba = getHash(xi + 1, yi + 1, zi);
	bbb = getHash(xi + 1, yi + 1, zi + 1);

	double x1, x2, x3, x4, y1, y2;
	
	x1 = lerp(grad(aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf), u);
	x2 = lerp(grad(aab, xf, yf, zf - 1), grad(bab, xf - 1, yf, zf - 1), u);
	x3 = lerp(grad(aba, xf, yf - 1, zf), grad(bba, xf - 1, yf - 1, zf), u);
	x4 = lerp(grad(abb, xf, yf - 1, zf - 1), grad(bbb, xf - 1, yf - 1, zf - 1), u);

	y1 = lerp(x1, x3, v);
	y2 = lerp(x2, x4, v);

	return (lerp(y1, y2, w) + 1) / 2;
}

double Noise3d::getPoint(size_t seed, double wavelength, double x, double y, double z) {
	x /= wavelength;
	y /= wavelength;
	z /= wavelength;

	long long xi = (long long)x;
	long long yi = (long long)y;
	long long zi = (long long)z;

	double xf = abs(x - xi);
	if(x < 0) {
		xf = 1 - xf;
		xi--;
	}
	double yf = abs(y - yi);
	if(y < 0) {
		yf = 1 - yf;
		yi--;
	}
	double zf = abs(z - zi);
	if(z < 0) {
		zf = 1 - zf;
		zi--;
	}

	double u = fade(xf);
	double v = fade(yf);
	double w = fade(zf);

	size_t aaa, aab, aba, abb, baa, bab, bba, bbb;
	aaa = getHash(xi, yi, zi, seed);
	aab = getHash(xi, yi, zi + 1, seed);
	aba = getHash(xi, yi + 1, zi, seed);
	abb = getHash(xi, yi + 1, zi + 1, seed);
	baa = getHash(xi + 1, yi, zi, seed);
	bab = getHash(xi + 1, yi, zi + 1, seed);
	bba = getHash(xi + 1, yi + 1, zi, seed);
	bbb = getHash(xi + 1, yi + 1, zi + 1, seed);

	double x1, x2, x3, x4, y1, y2;

	x1 = lerp(grad(aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf), u);
	x2 = lerp(grad(aab, xf, yf, zf - 1), grad(bab, xf - 1, yf, zf - 1), u);
	x3 = lerp(grad(aba, xf, yf - 1, zf), grad(bba, xf - 1, yf - 1, zf), u);
	x4 = lerp(grad(abb, xf, yf - 1, zf - 1), grad(bbb, xf - 1, yf - 1, zf - 1), u);

	y1 = lerp(x1, x3, v);
	y2 = lerp(x2, x4, v);

	return (lerp(y1, y2, w) + 1) / 2;
}

glm::vec3 Noise3d::getGrad(size_t seed, double wavelength, double x, double y, double z) {
	x /= wavelength;
	y /= wavelength;
	z /= wavelength;

	long long xi = (long long)x;
	long long yi = (long long)y;
	long long zi = (long long)z;

	double xf = abs(x - xi);
	if(x < 0) {
		xf = 1 - xf;
		xi--;
	}
	double yf = abs(y - yi);
	if(y < 0) {
		yf = 1 - yf;
		yi--;
	}
	double zf = abs(z - zi);
	if(z < 0) {
		zf = 1 - zf;
		zi--;
	}

	double u = fade(xf);
	double v = fade(yf);
	double w = fade(zf);

	size_t aaa, aab, aba, abb, baa, bab, bba, bbb;
	aaa = getHash(xi, yi, zi, seed);
	aab = getHash(xi, yi, zi + 1, seed);
	aba = getHash(xi, yi + 1, zi, seed);
	abb = getHash(xi, yi + 1, zi + 1, seed);
	baa = getHash(xi + 1, yi, zi, seed);
	bab = getHash(xi + 1, yi, zi + 1, seed);
	bba = getHash(xi + 1, yi + 1, zi, seed);
	bbb = getHash(xi + 1, yi + 1, zi + 1, seed);

	double x1, x2, x3, x4, y1, y2;

	x1 = lerp(grad(aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf), u);
	x2 = lerp(grad(aab, xf, yf, zf - 1), grad(bab, xf - 1, yf, zf - 1), u);
	x3 = lerp(grad(aba, xf, yf - 1, zf), grad(bba, xf - 1, yf - 1, zf), u);
	x4 = lerp(grad(abb, xf, yf - 1, zf - 1), grad(bbb, xf - 1, yf - 1, zf - 1), u);

	y1 = lerp(x1, x3, v);
	y2 = lerp(x2, x4, v);

	double fxyz = lerp(y1, y2, w);

	double dx, dy, dz;
	dx = dy = dz = 0.01;

	// X Direction
	double du = fade(xf + dx);
	x1 = lerp(grad(aaa, xf+dx, yf, zf), grad(baa, (xf + dx) - 1, yf, zf), du);
	x2 = lerp(grad(aab, xf + dx, yf, zf - 1), grad(bab, (xf + dx) - 1, yf, zf - 1), du);
	x3 = lerp(grad(aba, xf + dx, yf - 1, zf), grad(bba, (xf + dx) - 1, yf - 1, zf), du);
	x4 = lerp(grad(abb, xf + dx, yf - 1, zf - 1), grad(bbb, (xf + dx) - 1, yf - 1, zf - 1), du);

	y1 = lerp(x1, x3, v);
	y2 = lerp(x2, x4, v);

	double fxdxyz = lerp(y1, y2, w);

	// Y Direction
	x1 = lerp(grad(aaa, xf, yf + dy, zf), grad(baa, xf - 1, yf + dy, zf), u);
	x2 = lerp(grad(aab, xf, yf + dy, zf - 1), grad(bab, xf - 1, yf + dy, zf - 1), u);
	x3 = lerp(grad(aba, xf, (yf + dy) - 1, zf), grad(bba, xf - 1, (yf + dy) - 1, zf), u);
	x4 = lerp(grad(abb, xf, (yf + dy) - 1, zf - 1), grad(bbb, xf - 1, (yf + dy) - 1, zf - 1), u);

	double dv = fade(yf + dy);
	y1 = lerp(x1, x3, dv);
	y2 = lerp(x2, x4, dv);

	double fxydyz = lerp(y1, y2, w);

	// Z Direction
	x1 = lerp(grad(aaa, xf, yf, zf + dz), grad(baa, xf - 1, yf, zf + dz), u);
	x2 = lerp(grad(aab, xf, yf, (zf + dz) - 1), grad(bab, xf - 1, yf, (zf + dz) - 1), u);
	x3 = lerp(grad(aba, xf, yf - 1, zf + dz), grad(bba, xf - 1, yf - 1, zf + dz), u);
	x4 = lerp(grad(abb, xf, yf - 1, (zf + dz) - 1), grad(bbb, xf - 1, yf - 1, (zf + dz) - 1), u);

	y1 = lerp(x1, x3, v);
	y2 = lerp(x2, x4, v);

	double dw = fade(zf + dz);
	double fxyzdz = lerp(y1, y2, dw);

	return glm::vec3((fxdxyz - fxyz) / dx, (fxydyz - fxyz) / dy, (fxyzdz - fxyz) / dz);
}

