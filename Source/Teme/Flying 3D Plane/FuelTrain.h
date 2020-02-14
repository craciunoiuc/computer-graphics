class FuelTrain {
private:
	float fuelTrainOX;
	float fuelTrainOY;
	float fuelTrainOZ;
	int fuelPieces;
	std::vector<bool> hit;

public:

	// Builds an oscilating line of fuel triangles
	FuelTrain(float planeOZ) {
		fuelPieces = 3 + rand() % 5;
		for (int i = 0; i < fuelPieces; ++i) {
			hit.push_back(false);
		}
		fuelTrainOZ = planeOZ;
		fuelTrainOY = 2 + 6 * static_cast<float>(rand() % 100) / 100.0f;
		fuelTrainOX = 20 + 20 * static_cast<float>(rand() % 100) / 100.0f;

	}

	// Moves the fuel train on a random position on OY
	void JumblePosition() {
		fuelTrainOY = 2 + 6 * static_cast<float>(rand() % 100) / 100.0f;
	}

	// Moves the fuel train back to start
	void TeleportToStart() {
		fuelTrainOX = 20 + 20 * static_cast<float>(rand() % 100) / 100.0f;
		fuelPieces = 3 + rand() % 5;
		hit.clear();
		for (int i = 0; i < fuelPieces; ++i) {
			hit.push_back(false);
		}
	}

	// Builds the ModelMatrix for each fuel block
	std::vector<glm::mat4> RenderFuelTrain(glm::vec3 leftCorner, float rotationSea) {
		std::vector<glm::mat4> result;
		glm::mat4 modelMatrix;
		fuelTrainOX += leftCorner.x;
		for (int i = 0; i < fuelPieces; ++i) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform::Translate(fuelTrainOX + static_cast<float>(i) / 2.0f, 
				fuelTrainOY + sin(leftCorner.y + static_cast<float>(i) / 3.5f) / 2, fuelTrainOZ);
			modelMatrix *= Transform::RotateOZ(rotationSea);
			result.push_back(modelMatrix);
		}
		return result;
	}

	std::vector<bool>* getHit() {
		return &hit;
	}

	// Returns a vector with the position of each fuel block
	std::vector<glm::vec3> getCoords(glm::vec3 leftCorner, float rotationSea) {
		std::vector<glm::vec3> result;
		for (int i = 0; i < fuelPieces; ++i) {
			result.push_back(glm::vec3(leftCorner.x + fuelTrainOX + static_cast<float>(i) / 2.0f, 
				fuelTrainOY + sin(leftCorner.y + static_cast<float>(i) / 3.5f) / 2, fuelTrainOZ));
		}
		return result;
	}

	inline bool toRemove() {
		return (fuelTrainOX < -15);
	}
};