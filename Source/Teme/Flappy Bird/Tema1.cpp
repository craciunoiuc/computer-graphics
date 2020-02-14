#include "Tema1.h"

#include <vector>
#include <string>
#include <iostream>
#include "../../../Visual Studio/Objects.h"
#include "../../../Visual Studio/Transform.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}
static int cameraX = 0, cameraY = 0, cameraZ = 1;
static int inaltime = 0.0f, latime = 0.0f;
void Tema1::Init()
{
	renderCameraTarget = false;

	//camera = new Tema::Camera();
	glm::ivec2 resolution = window->GetResolution();
	//camera->Set(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));

	// TODO TO CHANGE
	camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0, 40);
	camera->SetPosition(glm::vec3(cameraX, cameraY, cameraZ));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	//
	//projectionMatrix = glm::ortho(0.0f, (float)resolution.x, 0.0f, (float)resolution.y, 0.01f, 40.0f);

	//camera->TranslateUpword(100);
	GetCameraInput()->SetActive(false);

	float length = 30;
	glm::vec3 centerSquare = glm::vec3(-length / 2, -length / 2, 0);
	glm::vec3 centerTriangle = glm::vec3(-length / 2, -length / 3, 0);
	glm::vec3 centerRectangle = glm::vec3(0, 0, 0); 

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	for (int i = 0; i < 50; ++i) {
		randomGap.push_back(rand() % 100);
	}

	Mesh* square = Objects::CreateSquare("square", centerSquare, length, glm::vec3(0, 0, 0), true);
	AddMeshToList(square);
	Mesh* rectangle = Objects::CreateRectangle("rectang", centerRectangle, length * 3, glm::vec3(0, 0, 1), true);
	AddMeshToList(rectangle);
	Mesh* triangle = Objects::CreateTriangle("triangle", centerTriangle, length, glm::vec3(0, 0, 0), true);
	AddMeshToList(triangle);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(1, 1, 1, 1); // Setat culoare fundal
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}
void Tema1::Update(float deltaTimeSeconds)
{
	//glm::ivec2 resolution = window->GetResolution();
	//projectionMatrix = glm::ortho(0.0f + latime, (float)resolution.x - latime, 0.0f + inaltime, (float)resolution.y - inaltime, 0.0f, 40.0f);
	const int startX = 150, startY = 350;
	if (gameStarted && !gameFinished) {
		translateX += (100 + velocity) * deltaTimeSeconds;
		if (toJump > 1) {
			translateY += 70 * deltaTimeSeconds;
			if (rotateAngle < 0.90) {
				rotateAngle += 1.55 * deltaTimeSeconds;
			}
			//rotateAngle += 1.2 * deltaTimeSeconds;
			toJump--;
		}
		else {
			translateY -= 60 * deltaTimeSeconds;
			if (rotateAngle > -0.90) {
				rotateAngle -= 1.05 * deltaTimeSeconds;
			}
		}
		if (rotateAngle < 0.7) {
			jumpKey = true;
		}
	}
	std::vector <std::pair<std::pair<int, int>, std::pair<int, int>>> birdCoords;

	// Corp
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(startX, startY);
	modelMatrix *= Transform::Translate(0, translateY);
	modelMatrix *= Transform::Translate(-30, 0);
	modelMatrix *= Transform::Rotate(rotateAngle);
	modelMatrix *= Transform::Translate(30, 0);
	RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
	birdCoords.push_back({ {startX, startY + translateY}, {30, 30} });

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(startX, startY);
	modelMatrix *= Transform::Translate(25, 0);
	modelMatrix *= Transform::Translate(0, translateY);
	modelMatrix *= Transform::Translate(-55, 0);
	modelMatrix *= Transform::Rotate(rotateAngle);
	modelMatrix *= Transform::Translate(55, 0);
	RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
	birdCoords.push_back({ {startX + 25, startY + translateY}, {30, 30} });

	// Picioare
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(startX, startY);
	modelMatrix *= Transform::Translate(0, translateY - 10);
	modelMatrix *= Transform::Translate(-30, 10);
	modelMatrix *= Transform::Rotate(rotateAngle);
	modelMatrix *= Transform::Translate(30, -10);
	RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
	birdCoords.push_back({ {startX, startY + translateY - 10}, {30, 30} });

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(startX, startY);
	modelMatrix *= Transform::Translate(25, 0);
	modelMatrix *= Transform::Translate(0, translateY - 10);
	modelMatrix *= Transform::Translate(-55, 10);
	modelMatrix *= Transform::Rotate(rotateAngle);
	modelMatrix *= Transform::Translate(55, -10);
	RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
	birdCoords.push_back({ {startX, startY + translateY - 10}, {30, 30} });

	//Aripi
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(startX, startY);
	modelMatrix *= Transform::Translate(0 + 20, translateY + 10);
	modelMatrix *= Transform::Rotate(0.1);
	modelMatrix *= Transform::Translate(-50, -10);
	modelMatrix *= Transform::Rotate(rotateAngle);
	modelMatrix *= Transform::Translate(50, 10);
	RenderMesh2D(meshes["triangle"], shaders["VertexColor"], modelMatrix);
	//birdCoords.push_back({ startX + 20, startY + translateY + 10});

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(startX, startY);
	modelMatrix *= Transform::Translate(0 + 1, translateY + 10);
	modelMatrix *= Transform::Rotate(0.1);
	modelMatrix *= Transform::Translate(-31, -10);
	modelMatrix *= Transform::Rotate(rotateAngle);
	modelMatrix *= Transform::Translate(31, 10);
	RenderMesh2D(meshes["triangle"], shaders["VertexColor"], modelMatrix);
	//birdCoords.push_back({ startX + 1, startY + 10 + translateY});

	//Cioc
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(startX, startY);
	modelMatrix *= Transform::Translate(0 + 41, translateY - 9);
	modelMatrix *= Transform::Translate(-71, 9);
	modelMatrix *= Transform::Rotate(rotateAngle);
	modelMatrix *= Transform::Translate(71, -9);
	RenderMesh2D(meshes["triangle"], shaders["VertexColor"], modelMatrix);
	//birdCoords.push_back({ { startX + 41, startY + translateY - 9}, { 30, 30 } });

	//Coada
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform::Translate(startX, startY);
	modelMatrix *= Transform::Translate(0 - 10, translateY - 5);
	modelMatrix *= Transform::Translate(-20, 5);
	modelMatrix *= Transform::Rotate(rotateAngle);
	modelMatrix *= Transform::Translate(20, -5);
	RenderMesh2D(meshes["triangle"], shaders["VertexColor"], modelMatrix);
	//birdCoords.push_back({ startX - 10, startY + translateY - 5});

	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> objectCoords;

	// Desenare mediu
	const int offsetX = 1100;
	const int offsetY = -450;

	bool newElem = true;
	// Pereche
	int nrOfCreatedBarriers = 0;
	for (uint32_t barrier = ((nrOfCreatedBarriers - 6) > 0)? (nrOfCreatedBarriers - 6) * 200 : 0; barrier + startX + offsetX - translateX < 1280; ++nrOfCreatedBarriers, barrier += 200) {

		if (barrier + startX + offsetX - translateX > -90) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform::Translate(barrier + startX + offsetX, startY + 70 + randomGap.at(nrOfCreatedBarriers % 50));
			modelMatrix *= Transform::Scale(1.0, 2.3);
			modelMatrix *= Transform::Translate(-translateX, 0);
			RenderMesh2D(meshes["rectang"], shaders["VertexColor"], modelMatrix);
			
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform::Translate(barrier + startX + offsetX, startY + offsetY + randomGap.at(nrOfCreatedBarriers % 50));
			modelMatrix *= Transform::Scale(1.0, 2.0);
			modelMatrix *= Transform::Translate(-translateX, 0);
			RenderMesh2D(meshes["rectang"], shaders["VertexColor"], modelMatrix);
			objectCoords.push_back({ {barrier + startX + offsetX - translateX, (startY + 60 + randomGap.at(nrOfCreatedBarriers % 50))}, {90, 180 * 2.3} });
			objectCoords.push_back({ {barrier + startX + offsetX - translateX, startY + offsetY + randomGap.at(nrOfCreatedBarriers % 50)}, {90, 360} });
		}
	}

	// Verificare coliziune
	for (auto& barrier : objectCoords) {
		for (auto& birdPart : birdCoords) {
			if (!gameFinished && Objects::ObjectsOverlap(birdPart, barrier) && Objects::ObjectsOverlap(birdPart, barrier)) {
				gameFinished = true;
				std::cout << "Score:" << nrOfCreatedBarriers - 6 << std::endl;
				break;
			}
		}
	}
}

void Tema1::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	float cameraSpeed = 2.0f;
	if (jumpKey && window->KeyHold(GLFW_KEY_SPACE)) {
		gameStarted = true;
		jumping = true;
		toJump = 20;
		jumpKey = false;
	}
	
	/*if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left

			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpward(-deltaTime * cameraSpeed);
		}
	}*/

	if (window->KeyHold(GLFW_KEY_3)) {
		inaltime += 0.5f;
	}

	if (window->KeyHold(GLFW_KEY_4)) {
		inaltime -= 0.5f;
	}

	if (window->KeyHold(GLFW_KEY_5)) {
		latime += 0.5f;
	}

	if (window->KeyHold(GLFW_KEY_6)) {
		latime -= 0.5f;
	}
}

void Tema1::OnKeyPress(int key, int mods)
{


	if (key == GLFW_KEY_E) {
		velocity += 200;
	}
	if (key == GLFW_KEY_Q) {
		velocity -= 200;
	}
	if (key == GLFW_KEY_R) {
		gameFinished = false;

	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}

// De Facut
// 1. Camera 2d
// 2. Curatat Cod
// 4. Readme
// 7. Modificat translate sa nu faca overflow - face la 10mil
