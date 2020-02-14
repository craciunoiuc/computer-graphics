#pragma once
#include <Component/SimpleScene.h>
#include <ctime>
#include "TemaCamera.h"
#include "CreateObjects.h"
#include "Transform3D.h"
#include "Clouds.h"
#include "FuelTrain.h"
#include "Obstacles.h"

#define RADIAN 0.0174532925
#define DEGREES 57.29577950
class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

		// Calculates if 2 spheres intersect based on their coordinates and radius using square root
		static bool SpheresIntersect(std::pair<glm::vec3, float> sphere, std::pair<glm::vec3, float> other) {
			float distance = sqrtf((sphere.first.x - other.first.x) * (sphere.first.x - other.first.x) +
					  			   (sphere.first.y - other.first.y) * (sphere.first.y - other.first.y) +
								   (sphere.first.z - other.first.z) * (sphere.first.z - other.first.z));
			return distance < (sphere.second + other.second);
		}

	protected:
		Tema::Camera *camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;
		std::vector<Cloud> clouds;
		std::vector<FuelTrain> fuelTrains;
		std::vector<Obstacle> obstacles;
		float planePosX = 0, planePosY = 0, planeRot = RADIANS(90), planeVelocity = 0;
		int scoreTimer = 0;
		int nrOfClouds = 15 + rand() % 5;
		float obstaclesMove = 0;
		float obstaclesFluctuate = 0;
		bool obstaclesFluctuateIncrease = true;
		float rotateValue = 0;
		float rotateValueOX = 0;
		float rotateClouds = 1;
		float sensitivity = 3.5f;
		float planeStartX = -5;
		float planeStartY = -1;
		const float planeStartZ = -7;
		float rotateAroundSea = 1;
		float rotatePropeller = 1;
		bool gameStarted = false;
		int nrLives = 3;
		bool gameFinished = false;
		bool resetPos = true;
		bool gameFinishedDone = false;
		float slowingDown = 0.7f;
		float fuel = 125.0f;
		float speedDificulty = 0.0f;
		float moveBackValue = 0.0f;
		glm::vec3 lightPosition;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		const int seed = rand();
};
