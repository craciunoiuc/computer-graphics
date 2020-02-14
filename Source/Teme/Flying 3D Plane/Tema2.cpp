#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	camera = new Tema::Camera();

	{
		Mesh* mesh = Objects::CreateCube("cube", glm::vec3(0, 0, 0), 
							1, glm::vec3(0.31, 0.55, 1));
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Sphere used for obstacles
	{
		Mesh* mesh = Objects::CreateSphere("Sphere", glm::vec3(0, 0, 0), 
							0.25f, glm::vec3(1, 0.55, 0.6));
		meshes[mesh->GetMeshID()] = mesh;
		obstacles.push_back(Obstacle(-6));
	}

	// Tetrahedron used in fuel trains
	{
		Mesh* mesh = Objects::CreateTetrahedron("FuelBlock", glm::vec3(0, 0, 0), 0.2f, 
								glm::vec3(0, 1, 0));
		meshes[mesh->GetMeshID()] = mesh;
		fuelTrains.push_back(FuelTrain(-6));
		fuelTrains.push_back(FuelTrain(-6));
	}

	// Cubes for each part of the plane
	{
		float cubeLength = 1;
		Mesh* mesh = Objects::CreateCube("PlaneBody", glm::vec3(5 + cubeLength/2, cubeLength/2, 
							cubeLength), cubeLength, glm::vec3(0.90, 0, 0));
		meshes[mesh->GetMeshID()] = mesh;

		cubeLength = 1;
		mesh = Objects::CreateCube("PlaneTail", glm::vec3(5 + cubeLength / 2, cubeLength / 2, 
							cubeLength), cubeLength, glm::vec3(0.85, 0, 0));
		meshes[mesh->GetMeshID()] = mesh;

		cubeLength = 1;
		mesh = Objects::CreateCube("PlaneHead", glm::vec3(5 + cubeLength / 2, cubeLength / 2, 
							cubeLength), cubeLength, glm::vec3(1, 1, 1));
		meshes[mesh->GetMeshID()] = mesh;

		cubeLength = 1;
		mesh = Objects::CreateCube("PlaneRotor", glm::vec3(5 + cubeLength / 2, cubeLength / 2, 
							cubeLength), cubeLength, glm::vec3(0.3, 0.3, 0.3));
		meshes[mesh->GetMeshID()] = mesh;

		cubeLength = 0.7f;
		mesh = Objects::CreateCube("PlaneWing", glm::vec3(5 + cubeLength / 2, cubeLength / 2, 
							cubeLength), cubeLength, glm::vec3(0.7, 0, 0));
		meshes[mesh->GetMeshID()] = mesh;

		cubeLength = 0.2f;
		mesh = Objects::CreateCube("PlanePropeller", glm::vec3(5 + cubeLength / 2, cubeLength / 2, 
							cubeLength), cubeLength, glm::vec3(0.1, 0.1, 0.1));
		meshes[mesh->GetMeshID()] = mesh;

		cubeLength = 1;
		mesh = Objects::CreateCube("PlaneTailFin", glm::vec3(5 + cubeLength / 2, cubeLength / 2, 
							cubeLength), cubeLength, glm::vec3(1, 0, 0));
		meshes[mesh->GetMeshID()] = mesh;

		cubeLength = 1;
		mesh = Objects::CreateCube("PlaneWheel", glm::vec3(5 + cubeLength / 2, cubeLength / 2, 
							cubeLength), cubeLength, glm::vec3(0.1, 0.1, 0.1));
		meshes[mesh->GetMeshID()] = mesh;

		cubeLength = 1;
		mesh = Objects::CreateCube("PlaneSeat", glm::vec3(5 + cubeLength / 2, cubeLength / 2, 
							cubeLength), cubeLength, glm::vec3(0.42f, 0.14f, 0.1f));
		meshes[mesh->GetMeshID()] = mesh;

		cubeLength = 1;
		mesh = Objects::CreateCube("PlaneWindow", glm::vec3(5 + cubeLength / 2, cubeLength / 2, 
							cubeLength), cubeLength, glm::vec3(1, 1, 1));
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Cube for cloud creation
	{
		float cubeLength = 0.1f;
		Mesh* mesh = Objects::CreateCube("cloudPiece", glm::vec3(cubeLength/2, cubeLength/2, 
								cubeLength/2), cubeLength, glm::vec3(0.94, 0.94, 0.94));
		meshes[mesh->GetMeshID()] = mesh;

		for (int i = 0; i < nrOfClouds; ++i) {
			clouds.push_back(Cloud(3 + rand() % 4));
		}
	}

	// Cylinder for the sea
	{
		float radius = 0.6f;
		float height = 16;
		Mesh* mesh = Objects::CreateCylinder("cylinder", glm::vec3(0, 0, -height / 2), 
							radius, height, glm::vec3(110.0/255.0, 170.0/255.0, 245.0/255.0));
		meshes[mesh->GetMeshID()] = mesh;
	}

	// A 2D triangle for the interface
	{
		float length = 1;
		Mesh* mesh = Objects::CreateTriangle2D("triangle", glm::vec3(length / 2, length / 3, 0), 
								length, glm::vec3(1, 140.0/255.0, 0));
		meshes[mesh->GetMeshID()] = mesh;
	}

	// A 2D square for the interface
	{
		float length = 1;
		Mesh* mesh = Objects::CreateSquare2D("FuelTotal", glm::vec3(length / 2, length / 2, 0), 
								length, glm::vec3(235.0 / 255.0, 182.0 / 255.0, 68.0 / 255.0));
		meshes[mesh->GetMeshID()] = mesh;
	}

	// A 2D square for the interface
	{
		float length = 1;
		Mesh* mesh = Objects::CreateSquare2D("FuelPercent", glm::vec3(0, 0, 0), length, 
							glm::vec3(235.0 / 255.0, 225.0 / 255.0, 68.0 / 255.0));
		meshes[mesh->GetMeshID()] = mesh;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(4.15f, 6.15f, 0);
		materialShininess = 300;
		materialKd = 0.5;
		materialKs = 0.5;
	}

	// Creates a shader to be used by the sea for the deformation
	{
		Shader* shader = new Shader("ShaderLightSea");
		shader->AddShader("Source/Teme/Tema2/Shaders/VertexShaderSea.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema2/Shaders/FragmentShaderSea.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Creates a shader that has lightning for all objects in the scene
	{
		Shader* shader = new Shader("ShaderLight");
		shader->AddShader("Source/Teme/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Tema2::FrameStart()
{
	glClearColor(1, 214.0/255.0, 194.0/255.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	// If the game is ongoing move the obstacles on OX, fluctuate them on OY, rotate the clouds
	// rotate the objects around the sea, rotate the propellers of the plane and rotate the plane
	// if necessary.
	if (!gameFinished && gameStarted) {
		obstaclesMove = (speedDificulty + 4) * deltaTimeSeconds;
		if (obstaclesFluctuateIncrease) {
			obstaclesFluctuate += 1.0f * deltaTimeSeconds;
			if (obstaclesFluctuate > 0.3f) {
				obstaclesFluctuateIncrease = false;
			}
		}
		else {
			obstaclesFluctuate -= 1.0f * deltaTimeSeconds;
			if (obstaclesFluctuate < -0.3f) {
				scoreTimer++;
				if (scoreTimer % 2 == 0) {
					obstacles.push_back(Obstacle(-6));
					speedDificulty += 0.15f;
				}
				obstaclesFluctuateIncrease = true;
			}
		}
		if (moveBackValue < 0.0f) {
			moveBackValue += 1.5f * deltaTimeSeconds;
		}
		else {
			moveBackValue = 0.0f;
		}
		rotateClouds += (speedDificulty/4 + 0.7f) * deltaTimeSeconds;
		rotateAroundSea += (speedDificulty/4 + 0.7f) * deltaTimeSeconds;
		rotatePropeller += 16 * deltaTimeSeconds;
		if (gameStarted) {
			fuel -= 10.15f * deltaTimeSeconds;
			if (rotateValue > 0.005f) {
				rotateValue -= 0.5f * deltaTimeSeconds;
			}
			else {
				if (rotateValue < -0.005f) {
					rotateValue += 0.5f * deltaTimeSeconds;
				}
				else {
					rotateValue = 0;
				}
			}
			if (planeVelocity > 0.005f) {
				planeVelocity -= 0.1f * deltaTimeSeconds;
			}
			else {
				planeVelocity = 0;
			}
		}
	}
	else {
		
		// If the game has finished, slow everything down, start moving the plane down,
		// rotate the plane on both OX and OZ and move it forward a little
		if (gameFinished) {
			obstaclesMove = 4 * slowingDown * deltaTimeSeconds;
			camera->FPP = false;
			if (resetPos) {
				resetPos = false;
				rotateValue = 0.0f;
			}
			if (rotateValueOX < RADIANS(50)) {
				rotateValueOX += 1.3f * deltaTimeSeconds;
			}
			if (rotateValue > -RADIANS(40)) {
				rotateValue -= 0.9f * deltaTimeSeconds;
			}
			if (planeStartX + planePosX < 1 && !gameFinishedDone) {
				planePosX += (7.5f + slowingDown) * deltaTimeSeconds;
			}
			else {
				if (planeStartX + planePosX > 3 && !gameFinishedDone) {
					planePosX -= (7.5f + slowingDown) * deltaTimeSeconds;
				}
			}
			if (planePosY > -25 && !gameFinishedDone) {
				planePosY -= 5.5f * deltaTimeSeconds;
			}
			else {
				gameFinishedDone = true;
				planePosX = 0;
				planePosY = 0;
				planeStartX = -2;
				planeStartY = -2;
			}
			rotateAroundSea += slowingDown * deltaTimeSeconds;
			rotatePropeller += (3.0f + 10.0f * slowingDown) * deltaTimeSeconds;
			if (slowingDown > 0.1f) {
				slowingDown -= 0.3f * deltaTimeSeconds;
			}
			else {
				if (slowingDown < 0) {
					slowingDown = 0.01f;
				}
			}
		}
	}

	glm::mat4 modelMatrix;

	// Set Camera
	if (camera->FPP) {
		camera->position = glm::vec3(planeStartX + 6.8f + planePosX + moveBackValue, planeStartY + 1.1f + planePosY, planeStartZ + 0.8f);
		camera->forward = glm::vec3(0.0322143f, -0.105355f, -0.993913f);
		camera->right = glm::vec3(0.999475f, 0, 0.0323943f);
		camera->up = glm::vec3(0.0034129f, 0.994435f, -0.1053f);
		camera->RotateFirstPerson_OY(-RADIANS(90));
		camera->TranslateForward(1.15f);
		camera->RotateFirstPerson_OX(rotateValue);
		camera->TranslateForward(-1.15f);
	}
	else {
		camera->position = glm::vec3(-0.0136797, 1.51564, 7.45895);
		camera->forward = glm::vec3(0.0322143, -0.105355, -0.993913);
		camera->right = glm::vec3(0.999475, 0, 0.0323943);
		camera->up = glm::vec3(0.0034129, 0.994435, -0.1053);
	}

	// Show Lives and Fuel on screen
	if (!camera->FPP) {
		// Render Fuel Total
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(4.78374f + 5.25f, 4.64905f + 2.45f, -8);
		modelMatrix *= Transform::Translate(5.2f, 0.6f, 0);
		modelMatrix *= Transform::RotateOZ(0.0131434f);
		modelMatrix *= Transform::Translate(-5.2f, -0.6f, 0);
		modelMatrix *= Transform::Scale(4, 1, 1);
		RenderMesh(meshes["FuelTotal"], shaders["VertexColor"], modelMatrix);

		// Render Fuel Show
		if (fuel > 0.0) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform::Translate(9.76778f + 5.25f, 5.20182f + 2.45f, -7.99f);
			modelMatrix *= Transform::Translate(5.2f, 0.6f, 0);
			modelMatrix *= Transform::RotateOZ(0.0131434f);
			modelMatrix *= Transform::Translate(-5.2f, -0.6f, 0);
			RenderMesh(meshes["FuelPercent"], shaders["VertexColor"], modelMatrix);

			if (fuel > 25.0) {
				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform::Translate(8.76778f + 5.25f, 5.20182f + 2.45f, -7.99f);
				modelMatrix *= Transform::Translate(5.2f, 0.6f, 0);
				modelMatrix *= Transform::RotateOZ(0.0131434f);
				modelMatrix *= Transform::Translate(-5.2f, -0.6f, 0);
				RenderMesh(meshes["FuelPercent"], shaders["VertexColor"], modelMatrix);

				if (fuel > 50.0) {
					modelMatrix = glm::mat4(1);
					modelMatrix *= Transform::Translate(7.76778f + 5.25f, 5.20182f + 2.45f, -7.99f);
					modelMatrix *= Transform::Translate(5.2f, 0.6f, 0);
					modelMatrix *= Transform::RotateOZ(0.0131434f);
					modelMatrix *= Transform::Translate(-5.2f, -0.6f, 0);
					RenderMesh(meshes["FuelPercent"], shaders["VertexColor"], modelMatrix);

					if (fuel > 75.0) {
						modelMatrix = glm::mat4(1);
						modelMatrix *= Transform::Translate(6.76778f + 5.25f, 5.20182f + 2.45f, -7.99f);
						modelMatrix *= Transform::Translate(5.2f, 0.6f, 0);
						modelMatrix *= Transform::RotateOZ(0.0131434f);
						modelMatrix *= Transform::Translate(-5.2f, -0.6f, 0);
						RenderMesh(meshes["FuelPercent"], shaders["VertexColor"], modelMatrix);

					}
				}
			}
		}
		else {
			gameFinished = true;
		}

		// Render Lives
		if (nrLives > 0) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform::Translate(-12.5705f - 1.67f, 4.56449f + 1.72f, -7.76332f);
			modelMatrix *= Transform::Translate(-0.134279f, 0.38494f, 0);
			RenderMesh(meshes["triangle"], shaders["VertexColor"], modelMatrix);


			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform::Translate(-12.5705f - 1.67f, 4.56449f + 1.72f, -7.76332f);
			modelMatrix *= Transform::Translate(0.92f, 0.86f, 0);
			modelMatrix *= Transform::RotateOZ(160.0f + 0.19752f);
			modelMatrix *= Transform::Translate(-0.92f, -0.86f, 0);
			RenderMesh(meshes["triangle"], shaders["VertexColor"], modelMatrix);

			if (nrLives > 1) {
				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform::Translate(-12.5705f + 1 - 1.67f, 4.56449f + 1.72f, -7.76332f);
				modelMatrix *= Transform::Translate(-0.134279f, 0.38494f, 0);
				RenderMesh(meshes["triangle"], shaders["VertexColor"], modelMatrix);


				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform::Translate(-12.5705f + 1 - 1.67f, 4.56449f + 1.72f, -7.76332f);
				modelMatrix *= Transform::Translate(0.92f, 0.86f, 0);
				modelMatrix *= Transform::RotateOZ(160.0f + 0.19752f);
				modelMatrix *= Transform::Translate(-0.92f, -0.86f, 0);
				RenderMesh(meshes["triangle"], shaders["VertexColor"], modelMatrix);

				if (nrLives > 2) {
					modelMatrix = glm::mat4(1);
					modelMatrix *= Transform::Translate(-12.5705f + 2 - 1.67f, 4.56449f + 1.72f, -7.76332f);
					modelMatrix *= Transform::Translate(-0.134279f, 0.38494f, 0);
					RenderMesh(meshes["triangle"], shaders["VertexColor"], modelMatrix);


					modelMatrix = glm::mat4(1);
					modelMatrix *= Transform::Translate(-12.5705f + 2 - 1.67f, 4.56449f + 1.72f, -7.76332f);
					modelMatrix *= Transform::Translate(0.92f, 0.86f, 0);
					modelMatrix *= Transform::RotateOZ(160.0f + 0.19752f);
					modelMatrix *= Transform::Translate(-0.92f, -0.86f, 0);
					RenderMesh(meshes["triangle"], shaders["VertexColor"], modelMatrix);
				}
			}
		}
		else {
			gameFinished = true;
		}
	}
	else {
		if (nrLives == 0 || fuel <= 0.0) {
			gameFinished = true;
			camera->FPP = false;
		}
	}

	// Render Plane
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::Translate(1.30f, 0, 0);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Scale(1, 1, 0.9f);
		modelMatrix *= Transform::Scale(1, 0.6f, 0.6f);
		RenderSimpleMesh(meshes["PlaneBody"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(0.90, 0, 0));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -1.3f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -1.3f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(3.55f, 0.3f, 0.225f);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Scale(0.5f, 0.4f, 0.4f);
		RenderSimpleMesh(meshes["PlaneTail"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(0.85, 0, 0));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -1.3f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -1.3f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(4.95f, 0.75f, 0.66f);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Scale(0.25f, 0.2f, 0.1f);
		RenderSimpleMesh(meshes["PlaneTailFin"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(1, 0, 0));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -1.3f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -1.3f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(7.25f, 0, 0);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Scale(0.1f, 0.6f, 0.54f);
		RenderSimpleMesh(meshes["PlaneHead"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(1, 1, 1));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -1.3f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -1.3f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(7.35f, 0.4f, 0.5f);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Scale(0.1f, 0.2f, 0.2f);
		RenderSimpleMesh(meshes["PlaneRotor"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(0.3, 0.3, 0.3));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -1.3f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -1.3f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(7.52f, -0.1f, 0.65f);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Translate(0, 0.7f, 0.12f);
		modelMatrix *= Transform::RotateOX(rotatePropeller);
		modelMatrix *= Transform::Translate(0, -0.7f, -0.12f);
		modelMatrix *= Transform::Scale(0.09f, 3.5f, 0.5f);
		RenderSimpleMesh(meshes["PlanePropeller"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(0.1, 0.1, 0.1));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -1.3f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -1.3f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(7.52f, -0.1f, 0.65f);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Translate(0, 0.7f, 0.12f);
		modelMatrix *= Transform::RotateOX(RADIANS(90) + rotatePropeller);
		modelMatrix *= Transform::Translate(0, -0.7f, -0.12f);
		modelMatrix *= Transform::Scale(0.09f, 3.5f, 0.5f);
		RenderSimpleMesh(meshes["PlanePropeller"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(0.1, 0.1, 0.1));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -1.3f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -1.3f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(1.7f, 0.8f, -0.5f);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Scale(1, 0.1f, 2);
		RenderSimpleMesh(meshes["PlaneWing"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(0.7f, 0, 0));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -1.3f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -1.3f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(1.7f, 0.8f, -2.1f);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Scale(1, 0.1f, 2);
		RenderSimpleMesh(meshes["PlaneWing"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(0.7f, 0, 0));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -1.3f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -1.3f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(5.426f, 0.839f, 0.494f);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Scale(0.25f, 0.05f, 0.2f);
		RenderSimpleMesh(meshes["PlaneSeat"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(0.42, 0.14, 0.1));

		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(planeStartX, planeStartY, planeStartZ);
		modelMatrix *= Transform::RotateOX(rotateValueOX);
		modelMatrix *= Transform::Translate(5.5f - -1.3f - -planePosX + 1, 0, 0);
		modelMatrix *= Transform::RotateOZ(rotateValue);
		modelMatrix *= Transform::Translate(-5.5f + -1.3f + -planePosX + -1, 0, 0);
		modelMatrix *= Transform::Translate(6.944f, 0.742f, 0.418f);
		modelMatrix *= Transform::Translate(planePosX + moveBackValue, planePosY, 0);
		modelMatrix *= Transform::Scale(0.03f, 0.18f, 0.25f);
		RenderSimpleMesh(meshes["PlaneWindow"], shaders["ShaderLight"], modelMatrix, 
			glm::vec3(1, 1, 1));
	}

	// Render Extra fuel
	for (auto& fuel : fuelTrains) {
		if (fuel.toRemove()) {
			fuel.JumblePosition();
			fuel.TeleportToStart();
		}
		else {
			int nr = 0;
			for (auto& fuelPiece : fuel.RenderFuelTrain(glm::vec3(-obstaclesMove, 
											rotateAroundSea, 0), rotateAroundSea)) {
				if (!fuel.getHit()->at(nr)) {
					if (SpheresIntersect({ fuel.getCoords(glm::vec3(
						-obstaclesMove, rotateAroundSea, 0), rotateAroundSea)[nr], 0.25f }, 
						{ glm::vec3(planeStartX + 7.8f + planePosX, planeStartY + 0.5f + 
							planePosY, planeStartZ + 0.5f), 0.4f })) {
						fuel.getHit()->at(nr) = true;
						if (Tema2::fuel < 125) {
							Tema2::fuel += 25;
						}
					}
					else {
						RenderSimpleMesh(meshes["FuelBlock"], shaders["ShaderLight"], 
										fuelPiece, glm::vec3(0, 1, 0));
					}
				}
				nr++;
			}
		}
	}

	// Render Sea
	{
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(0, 0.9f, 0);
		modelMatrix *= Transform::RotateOZ(rotateAroundSea);
		RenderSimpleMesh(meshes["cylinder"], shaders["ShaderLightSea"], modelMatrix, 
						 glm::vec3(110.0 / 255.0, 170.0 / 255.0, 245.0 / 255.0));
	}
	
	// Render Clouds
	{
		for (auto& cloud : clouds) {
			if (cloud.toRemove(rotateAroundSea)) {
				cloud.JumbleCloud();
				cloud.SkipRotate(rotateAroundSea);
			}
			else {
				for (auto& piece : cloud.RenderCloud(glm::vec3(0, -0.5f, -6), rotateClouds, 
					rotateAroundSea)) {
					RenderSimpleMesh(meshes["cloudPiece"], shaders["ShaderLight"], piece, 
						glm::vec3(0.94f, 0.94f, 0.94f));
				}
			}
		}
	}

	// Render Obstacles
	{
		for (auto& obstacle : obstacles) {
			if (obstacle.toRemove()) {
				obstacle.JumblePosition();
				obstacle.TeleportToStart();
			}
			else {
				if (SpheresIntersect({obstacle.getCoords(), 0.5f}, {glm::vec3(planeStartX + 7.8f +
						planePosX, planeStartY + 0.5f + planePosY, planeStartZ + 0.5f), 0.4f})) {
					obstacle.JumblePosition();
					obstacle.TeleportToStart();
					nrLives--;
					rotateValue -= RADIANS(20);
					moveBackValue -= 1;
					continue;
				}
				else {
					RenderSimpleMesh(meshes["Sphere"], shaders["VertexColor"], 
						obstacle.RenderObstacle(glm::vec3(-obstaclesMove, obstaclesFluctuate, 0), 
							rotateAroundSea), glm::vec3(1, 0.55f, 0.6f));
				}
			}
		}
	}
}

void Tema2::FrameEnd()
{
}

// Render Mesh function for objects without lightining
void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform2f(glGetUniformLocation(shader->program, "seed"), rand(), 1);
	
	mesh->Render();
}

// Render Mesh function for objects that have lightning
void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, 
							 const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

	GLint locationLightPosition = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(locationLightPosition, 1, glm::value_ptr(lightPosition));

	GLint locationEyePosition = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(locationEyePosition, 1, glm::value_ptr(camera->getPosition()));

	GLint locationMaterial = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(locationMaterial, materialShininess);

	GLint locationMaterialKd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(locationMaterialKd, materialKd);

	GLint locationMaterialKs = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(locationMaterialKs, materialKs);

	GLint locObject = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(locObject, 1, glm::value_ptr(color));

	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	/*int randi = glGetUniformLocation(shader->program, "rand");
	glUniform1i(randi, time(NULL));*/

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema2::OnKeyPress(int key, int mods)
{
	// Starts the game
	if (!gameStarted && key == GLFW_KEY_SPACE) {
		gameStarted = true;
		planeStartX = -10.6523f;
		planeStartY = 2.719674f;
		rotateValue = RADIANS(-40);
	}

	// Changes the perspective
	if (gameStarted && key == GLFW_KEY_V) {
		camera->FPP = !camera->FPP;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// If the game is ongoing get input from the mouse
	if (!gameFinished && gameStarted) {
		if (planeStartY + planePosY - (sensitivity * (static_cast <float> (deltaY) / 400.0f)) > 1.3f &&
			planeStartY + planePosY - (sensitivity * (static_cast <float> (deltaY) / 400.0f)) < 6.2f) {
			planePosY -= sensitivity * (static_cast <float> (deltaY) / 400.0f);
		}
		rotateValue -= (static_cast <float> (deltaY) / 700.0f);
	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}

// Intrebari
// Lumini? - Putere?

// TODO
// 1. Mutat mai in spate avionul la impact
// 2. SHADER MARE DEFORMARE
// 4. OPTIMIZARI COD :)

