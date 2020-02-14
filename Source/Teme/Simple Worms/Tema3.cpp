#include "Tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <Core/Engine.h>

using namespace std;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
	camera = new Tema::Camera();

	// The Heightmap
	heightPixels = stbi_load("Source/Teme/Tema3/Textures/heightmap.png", &width, &height, &channels, STBI_grey);
	textureIDD = 0;
	{
		glGenTextures(1, &textureIDD);
		glBindTexture(GL_TEXTURE_2D, textureIDD);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		CheckOpenGLError();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, heightPixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		CheckOpenGLError();
		save = new Texture2D();
		save->Init(textureIDD, width, height, channels);
		mapTextures["save"] = save;
	}
	
	// Character texture
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Teme/Tema3/Textures/crate.jpg", GL_REPEAT);
		mapTextures["crate"] = texture;
	}

	// Ground texture
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Teme/Tema3/Textures/dirt.jpg", GL_REPEAT);
		mapTextures["dirt"] = texture;
	}

	// Sphere for the projectile
	{
		Mesh* mesh = Objects::CreateSphere("sphere", glm::vec3(0, 0, 0),
			1, glm::vec3(1, 1, 1));
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Plane mesh with extra spheres on top of it to check for colisions
	{
		Mesh* mesh = Objects::CreatePlane("plane", glm::vec3(0, 0, 0), terrainSize, glm::vec3(0, 0, 0), heightPixels);
		meshes[mesh->GetMeshID()] = mesh;
		int i = 0;
		for (auto& position : mesh->positions) {
			int x = i / 256;
			int y = i % 256;
			miniSpheres.push_back({position + glm::vec3(0, (float(heightPixels[y * 256 + x]) / 256. ) * 256. / 30. - 2, 0), 1});
			i++;
		}
	}

	// Box mesh for the characters
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(0, 1, 1);
		lightDirection = glm::vec3(0, -1, 0);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;

		typeOfLight = 0;
		angleOX = 0.f;
		angleOY = 0.f;
		cutoffAngle = 30.f;
	}

	// Shader for the ground
	{
		Shader* shader = new Shader("test");
		shader->AddShader("Source/Teme/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Teme/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Player1 starts so the camera is behind him
	camera->forward = glm::vec3(0.962746, -0.175635, -0.205602);
	camera->right = glm::vec3(0.208848, 0, 0.977948);
	camera->up = glm::vec3(0.171762, 0.984455, -0.0366809);
	camera->position = glm::vec3(9.3322 - 0.8162, 4.67107 + 0.3066, 15.934 + 0.1472);

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Tema3::FrameStart()
{
	glClearColor(1, 214.0/255.0, 194.0/255.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds)
{
	// Set the light on the players
	lightPosition = glm::vec3(9.5672, 6.99, 16.354);
	dX += deltaTimeSeconds * 2 * toReach.x;
	dY += deltaTimeSeconds * 2 * toReach.y;
	dZ += deltaTimeSeconds * 2 * toReach.z;
	sphereSpeed += deltaTimeSeconds * 4;

	// Stop after a set distance
	if (dX > 20 || dY > 20 || dZ > 20) {
		dX = dY = dZ = 0;
		sphereLaunched1 = sphereLaunched2 = false;
		overview = false;
	}
	if (sphereLaunched1 || sphereLaunched2) {
		glm::vec3 aux;
		if (sphereLaunched1) {
			aux = glm::vec3(9.3322 + dX, dY + 4.67107 + 5, dZ + 12.934);
		}
		if (sphereLaunched2) {
			aux = glm::vec3(25.4857 + dX, 1.72028 + dY + 5, 12.9875 + dZ);
		}
		int sphereCrt = 0;

		// See if projectile collides with other spheres with the origin in the vertexes
		for (auto& sphere : miniSpheres) {
			if (SpheresIntersect({ aux, 1 }, sphere)) {
				sphereLaunched1 = sphereLaunched2 = false;
				overview = false;

				int j = sphereCrt / 256.;
				int i = sphereCrt % 256;

				heightPixels[i * 256 + j] -= 20;
				heightPixels[i * 256 + j - 1] -= 20;
				heightPixels[i * 256 + j + 1] -= 20;

				heightPixels[i * 256 + j - 256] -= 20;
				heightPixels[i * 256 + j - 256 - 1] -= 20;
				heightPixels[i * 256 + j - 256 + 1] -= 20;

				heightPixels[i * 256 + j + 256] -= 250;
				heightPixels[i * 256 + j + 256 - 1] -= 20;
				heightPixels[i * 256 + j + 256 + 1] -= 20;

				heightPixels[i * 256 + j + 256 * 2] -= 20;
				heightPixels[i * 256 + j + 256 * 2 - 1] -= 20;
				heightPixels[i * 256 + j + 256 * 2 + 1] -= 20;

				heightPixels[i * 256 + j + 256 * 3] -= 20;
				heightPixels[i * 256 + j + 256 * 3 - 1] -= 20;
				heightPixels[i * 256 + j + 256 * 3 + 1] -= 20;

				heightPixels[i * 256 + j + 256 * 4] -= 20;
				heightPixels[i * 256 + j + 256 * 4 - 1] -= 20;
				heightPixels[i * 256 + j + 256 * 4 + 1] -= 20;

				heightPixels[i * 256 + j + 256 * 5] -= 20;
				heightPixels[i * 256 + j + 256 * 5 - 1] -= 20;
				heightPixels[i * 256 + j + 256 * 5 + 1] -= 20;

				glBindTexture(GL_TEXTURE_2D, textureIDD);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, heightPixels);
				mapTextures["save"]->Init(textureIDD, width, height, channels);

				dX = dY = dZ = 0;
				toReach = glm::vec3(0, 0, 0);
				break;
			}
			sphereCrt++;
		}
	}
	// Corp 1
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(9.3322, 4.87107 - 0.2, 15.934);
		modelMatrix *= Transform::RotateOY(cameraRotationObjectY);
		modelMatrix *= Transform::Scale(0.3, 0.3, 0.3);
		RenderSimpleMesh(meshes["box"], shaders["Simple"], modelMatrix, mapTextures["crate"]);
	}
	// Arma 1
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(9.3322, 4.87107, 15.72);
		modelMatrix *= Transform::Translate(0, -0.2, +0.214);
		modelMatrix *= Transform::RotateOY(cameraRotationObjectY);
		modelMatrix *= Transform::Translate(0, +0.2, -0.214);
		modelMatrix *= Transform::Scale(0.8, 0.2, 0.2);
		RenderSimpleMesh(meshes["box"], shaders["Simple"], modelMatrix, mapTextures["crate"]);
	}
	// Rocket 1
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(9.3322 + 0.284, 4.67107 + 0.2, 15.934 - 0.2179);
		if (sphereLaunched1) {
			modelMatrix *= Transform::Translate(dX, dY, dZ);
			if (overview) {
				camera->position = glm::vec3(9.3322 + dX, dY + 4.67107 + 5, dZ + 12.934);
			}
		}
		modelMatrix *= Transform::Translate(-0.284, -0.2, +0.2179);
		modelMatrix *= Transform::RotateOY(cameraRotationObjectY);
		modelMatrix *= Transform::Translate(0.284, +0.2, -0.2179);
		modelMatrix *= Transform::Scale(0.1, 0.1, 0.1);
		RenderSimpleMesh(meshes["sphere"], shaders["Simple"], modelMatrix, mapTextures["crate"]);
	}

	// Corp 2
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(27.6997 - 0.215, 1.72028, 17.2525 - 0.265);
		modelMatrix *= Transform::RotateOY(cameraRotationObjectY2);
		modelMatrix *= Transform::Scale(0.3, 0.3, 0.3);
		RenderSimpleMesh(meshes["box"], shaders["Simple"], modelMatrix, mapTextures["crate"]);
	}
	// Arma 2
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(27.6997 - 0.215, 1.72028 + 0.2, 17.2525 - 0.05);
		modelMatrix *= Transform::Translate(0, -0.2, -0.217);
		modelMatrix *= Transform::RotateOY(cameraRotationObjectY2);
		modelMatrix *= Transform::Translate(0, +0.2, +0.217);
		modelMatrix *= Transform::Scale(0.8, 0.2, 0.2);
		RenderSimpleMesh(meshes["box"], shaders["Simple"], modelMatrix, mapTextures["crate"]);
	}
	// Rocket 2
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= Transform::Translate(27.4857 - 0.3, 1.72028 + 0.2, 16.9875 + 0.217);
		if (sphereLaunched2) {
			modelMatrix *= Transform::Translate(dX, dY, dZ);
			if (overview) {
				camera->position = glm::vec3(25.4857 + dX, 1.72028 + dY + 5, 12.9875 + dZ);
			}
		}
		modelMatrix *= Transform::Translate(0.3, -0.2, -0.217);
		modelMatrix *= Transform::RotateOY(cameraRotationObjectY2);
		modelMatrix *= Transform::Translate(-0.3, +0.2, +0.217);
		modelMatrix *= Transform::Scale(0.1, 0.1, 0.1);
		RenderSimpleMesh(meshes["sphere"], shaders["Simple"], modelMatrix, mapTextures["crate"]);
	}

	// Plane
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		RenderSimpleMesh(meshes["plane"], shaders["test"], modelMatrix, mapTextures["dirt"]);
	}
}

void Tema3::FrameEnd()
{
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);


	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	glm::vec3 eyePosition = camera->getPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	float material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	float material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1f(material_shininess, materialShininess);

	GLint cut_off_angle = glGetUniformLocation(shader->program, "cut_off_angle");
	glUniform1f(cut_off_angle, cutoffAngle);
	
	float terrainSz = glGetUniformLocation(shader->program, "terrain_size");
	glUniform1f(terrainSz, terrainSize);

	if (texture1)	
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);


		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mapTextures["save"]->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "heightPixels"), 1);
	}

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	
	// Camera movement for debugging
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;
		if (window->KeyHold(GLFW_KEY_W)) {
			camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpward(-deltaTime * cameraSpeed);
		}
	}
}


void Tema3::OnKeyPress(int key, int mods)
{
	// Change player after analyzing shot
	if (key == GLFW_KEY_SPACE && !overview) {
		playerOneTurn = !playerOneTurn;
		if (playerOneTurn) {
			camera->forward = glm::vec3(0.962746, -0.175635, -0.205602);
			camera->right = glm::vec3(0.208848, 0, 0.977948);
			camera->up = glm::vec3(0.171762, 0.984455, -0.0366809);
			camera->position = glm::vec3(9.3322 - 0.8162, 4.67107 + 0.3066, 15.934 + 0.1472);
			cameraRotationObjectY2 = 0;
		}
		else {
			camera->forward = glm::vec3(-0.989263, -0.047531, 0.138203);
			camera->right = glm::vec3(-0.138359, 0, -0.990382);
			camera->up = glm::vec3(-0.0470739, 0.99887, 0.00657634);
			camera->position = glm::vec3(28.2923, 1.95425, 16.8657);
			cameraRotationObjectY = 0;
		}
	}

	// Shoot
	if (key == GLFW_KEY_LEFT_ALT) {
		if (playerOneTurn && !sphereLaunched1) {
			toReach = normalize(camera->GetTargetPosition() - camera->position);
			dX = dY = dZ = 0;
			sphereLaunched1 = true;
		}
		else {
			if (!playerOneTurn && !sphereLaunched2) {
				toReach = camera->GetTargetPosition() - camera->position;
				dX = dY = dZ = 0;
				sphereLaunched2 = true;
			}
		}
		overview = true;
		camera->forward = glm::vec3(0.285408, -0.767255, 0.574336);
		camera->right = glm::vec3(-0.895522, 0, 0.445017);
		camera->up = glm::vec3(0.341442, 0.641342, 0.687094);
	}

	// Test Deformability
	if (key == GLFW_KEY_4) {
		int sphereCrt = 257;
		int j = sphereCrt / 256.;
		int i = sphereCrt % 256;
		heightPixels[i * 256 + j] = 250;
		heightPixels[i * 256 + j - 1] = 250;
		heightPixels[i * 256 + j + 1] = 250;

		heightPixels[i * 256 + j - 256] = 250;
		heightPixels[i * 256 + j - 256 - 1] = 250;
		heightPixels[i * 256 + j - 256 + 1] = 250;

		heightPixels[i * 256 + j + 256] = 250;
		heightPixels[i * 256 + j + 256 - 1] = 250;
		heightPixels[i * 256 + j + 256 + 1] = 250;

		heightPixels[i * 256 + j + 256 * 2] = 250;
		heightPixels[i * 256 + j + 256 * 2 - 1] = 250;
		heightPixels[i * 256 + j + 256 * 2 + 1] = 250;

		heightPixels[i * 256 + j + 256 * 3] = 250;
		heightPixels[i * 256 + j + 256 * 3 - 1] = 250;
		heightPixels[i * 256 + j + 256 * 3 + 1] = 250;

		heightPixels[i * 256 + j + 256 * 4] = 250;
		heightPixels[i * 256 + j + 256 * 4 - 1] = 250;
		heightPixels[i * 256 + j + 256 * 4 + 1] = 250;

		heightPixels[i * 256 + j + 256 * 5] = 250;
		heightPixels[i * 256 + j + 256 * 5 - 1] = 250;
		heightPixels[i * 256 + j + 256 * 5 + 1] = 250;
		glBindTexture(GL_TEXTURE_2D, textureIDD);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, heightPixels);
		mapTextures["save"]->Init(textureIDD, width, height, channels);
	}
}

void Tema3::OnKeyRelease(int key, int mods)
{
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Rotate camera for aiming
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensitivityOX = 0.001f;
		float sensitivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			if (!overview) {
				camera->RotateFirstPerson_OX(-sensitivityOY * deltaY);
				if (playerOneTurn) {
					cameraRotationObjectY += -sensitivityOX * deltaX;
				}
				else {
					cameraRotationObjectY2 += -sensitivityOX * deltaX;
				}
				camera->TranslateForward(0.8);
				camera->RotateFirstPerson_OY(-sensitivityOX * deltaX);
				camera->TranslateForward(-0.8);
			}
		}
	}
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
