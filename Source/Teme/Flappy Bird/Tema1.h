#pragma once
#include <Component/SimpleScene.h>
#include "../../../Visual Studio/TemaCamera.h"
#include <Core/Engine.h>
#include <deque>
#include <vector>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

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
	void RenderMesh2DOwn(Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix, Tema::Camera* camera)
	{
		if (!mesh || !shader || !shader->program)
			return;

		shader->Use();
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glm::mat3 mm = modelMatrix;
		glm::mat4 model = glm::mat4(
			mm[0][0], mm[0][1], mm[0][2], 0.f,
			mm[1][0], mm[1][1], mm[1][2], 0.f,
			0.f, 0.f, mm[2][2], 0.f,
			mm[2][0], mm[2][1], 0.f, 1.f);

		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
		mesh->Render();
	}
protected:
	EngineComponents::Camera* camera; //TODO TO CHANGE
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;
	float length;
	int toJump = 0;
	bool jumping = false;
	bool gameFinished = false;
	bool gameStarted = false;
	Tema::ViewportSpace viewSpace;
	Tema::LogicSpace logicSpace;
	glm::mat3 modelMatrix, visMatrix;
	std::deque<uint8_t> randomGap;
	float translateX = 0, translateY = 0;
	float rotateAngle = 0;
	int velocity = 0;
	int score = 0;
	bool check = true;
	bool jumpKey = true;
};
