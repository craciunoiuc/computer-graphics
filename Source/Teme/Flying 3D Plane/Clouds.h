#pragma once

#include <Component/SimpleScene.h>
#include "Transform3D.h"
#define DEGREES 57.29577950

class Cloud {
private:
	int nrOfBlocks;
	float cloudStartPosRot;
	float cloudStartPosZ;
	std::vector<std::pair<glm::vec3, glm::vec3>> randomPositionAndScale;
	std::vector<std::pair<float, uint8_t>> randomAngleAndAxis;

public:
	// Assigns random positions to cloud pieces on creation
	Cloud(int nrOfBlocks) {
		this->nrOfBlocks = nrOfBlocks;
		cloudStartPosRot = rand() % 100;
		cloudStartPosZ = -5 + rand() % 15;
		for (int i = 0; i < nrOfBlocks; ++i) {
			randomPositionAndScale.push_back({glm::vec3(0.1 + 0.05 * static_cast<float> (rand()) / static_cast<float> (RAND_MAX),
													    0.1 + 0.05 * static_cast<float> (rand()) / static_cast<float> (RAND_MAX),
													    0),
											 glm::vec3(1 + static_cast<float> (rand()) / static_cast<float> (RAND_MAX), 
													   1 + static_cast<float> (rand()) / static_cast<float> (RAND_MAX), 
													   1)});
			randomAngleAndAxis.push_back({ 0.5 + static_cast<float> (rand()) / static_cast<float> (RAND_MAX), rand() % 4});
		}
	}
	
	~Cloud(){}

	// Assigns the clouds new positions
	void JumbleCloud() {
		randomAngleAndAxis.empty();
		randomAngleAndAxis.empty();
		for (int i = 0; i < nrOfBlocks; ++i) {
			randomPositionAndScale.push_back({ glm::vec3(0.1 + 0.05 * static_cast<float> (rand()) / static_cast<float> (RAND_MAX),
														0.1 + 0.05 * static_cast<float> (rand()) / static_cast<float> (RAND_MAX),
														0),
											 glm::vec3(1 + static_cast<float> (rand()) / static_cast<float> (RAND_MAX),
													   1 + static_cast<float> (rand()) / static_cast<float> (RAND_MAX),
													   1) });
			randomAngleAndAxis.push_back({ 0.5 + static_cast<float> (rand()) / static_cast<float> (RAND_MAX), rand() % 4 });
		}
	}

	// Does not move the clouds under the sea just skips over
	void SkipRotate(float rotationSea) {
		cloudStartPosRot += RADIANS(40);
	}

	// Generates the modelMatrix for each piece and returns it
	std::vector<glm::mat4> RenderCloud(glm::vec3 leftCorner, float rotationOwn, float rotationSea) {
		std::vector<glm::mat4> cloudPieces;
		glm::mat4 modelMatrix;

		for (int i = 0; i < nrOfBlocks; ++i) {
				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform::Translate(leftCorner.x, leftCorner.y, 
													leftCorner.z + cloudStartPosZ);
				modelMatrix *= Transform::Translate(0, 3, 0);
				modelMatrix *= Transform::RotateOZ(cloudStartPosRot + rotationSea);
				modelMatrix *= Transform::Translate(0, -3, 0);
				modelMatrix *= Transform::Translate(randomPositionAndScale[i].first.x,
					randomPositionAndScale[i].first.y, randomPositionAndScale[i].first.z);
				modelMatrix *= Transform::Scale(randomPositionAndScale[i].second.x,
					randomPositionAndScale[i].second.y, randomPositionAndScale[i].second.z);
				modelMatrix *= Transform::RotateAll(rotationOwn + randomAngleAndAxis[i].first, 
								randomAngleAndAxis[i].second);
				cloudPieces.push_back(modelMatrix);
		}
		return cloudPieces;
	}

	// Checks if block should be rendered
	inline bool toRemove(float rotationSea) {
		int rotateAroundSeaAngleTemporary = static_cast<int> ((rotationSea + cloudStartPosRot) * DEGREES) % 360;
		return !(rotateAroundSeaAngleTemporary > 20 && rotateAroundSeaAngleTemporary < 330);
	}

	float getCloudStartPosRot() {
		return cloudStartPosRot;
	}

	float getCloudStartPosZ() {
		return cloudStartPosZ;
	}
};