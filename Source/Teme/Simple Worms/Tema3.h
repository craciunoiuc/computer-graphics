#pragma once
#include <Component/SimpleScene.h>
#include <stb/stb_image.h>
#include <ctime>
#include "TemaCamera.h"
#include "CreateObjects.h"
#include "Transform3D.h"

#define RADIAN 0.0174532925
#define DEGREES 57.29577950
class Tema3 : public SimpleScene
{
	public:
		Tema3();
		~Tema3();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1);

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
		glm::vec3 lightPosition;
		glm::vec3 lightDirection;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		GLint typeOfLight;
		GLfloat angleOX, angleOY;
		GLfloat cutoffAngle;
		const int seed = rand();
		std::vector<std::pair<glm::vec3, float>> miniSpheres;
		std::unordered_map<std::string, Texture2D*> mapTextures;
		float cameraRotationObjectY = 0, cameraRotationObjectY2 = 0;
		float sphereSpeed = 5;
		bool  sphereLaunched1 = false;
		bool  sphereLaunched2 = false;
		bool  overview = false;
		glm::vec3 toReach;
		float dX = 0, dY = 0, dZ = 0;
		bool playerOneTurn = true;
		float terrainSize = 0.2;
		Texture2D* save;
		stbi_uc* heightPixels;
		int width, height, channels;
		GLuint textureIDD;
};
