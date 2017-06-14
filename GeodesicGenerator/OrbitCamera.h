#pragma once
#include "Camera.h"
class OrbitCamera:
	public Camera {
	glm::vec3 Center;
	float Distance;

	void updatePosition();
public:
	OrbitCamera(glm::vec3 center);
	~OrbitCamera();

	void update() override;
	void distanceOffset(float offset) { Distance += offset;
										Distance = glm::max(Distance, 1.1f);
	}
};

