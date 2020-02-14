#pragma once

#include <cmath>

class Obstacle {
private:
	float obstacleOZ = 0;
	float obstacleOY = 0;
	float obstacleOX = 0;

public:

	Obstacle(float planeOZ) {
		obstacleOZ = planeOZ;
		obstacleOY = 2 + 3.5f * static_cast<float>(rand() % 100) / 100.0f;
		obstacleOX = 25;
	}

	void JumblePosition() {
		obstacleOY = 2.5f + 3.5f * static_cast<float>(rand() % 100) / 100.0f;
	}

	void TeleportToStart() {
		obstacleOX = 25;
	}

	glm::mat4 RenderObstacle(glm::vec3 leftCorner, float rotationSea) {
		glm::mat4 modelMatrix;
		obstacleOX += leftCorner.x;
		modelMatrix *= Transform::Translate(obstacleOX, leftCorner.y + obstacleOY, leftCorner.z + obstacleOZ);
		modelMatrix *= Transform::RotateOZ(rotationSea);
		return modelMatrix;
	}

	inline bool toRemove() {
		return (obstacleOX < -15);
	}

	glm::vec3 getCoords() {
		return glm::vec3(obstacleOX, obstacleOY, obstacleOZ);
	}
};
