#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace Tema
{
	struct ViewportSpace
	{
		ViewportSpace() : x(0), y(0), width(1), height(1) {}
		ViewportSpace(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height) {}
		int x;
		int y;
		int width;
		int height;
	};

	struct LogicSpace
	{
		LogicSpace() : x(0), y(0), width(1), height(1) {}
		LogicSpace(float x, float y, float width, float height)
			: x(x), y(y), width(width), height(height) {}
		float x;
		float y;
		float width;
		float height;
	};

	class Camera
	{
	public:
		Camera()
		{
			position = glm::vec3(-10, -10, 0);
			forward = glm::vec3(-10, -10, 0);
			up = glm::vec3(-10, -10, 0);
			right = glm::vec3(-10, -10, 0);
			distanceToTarget = 50;
		}

		Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
		{
			Set(position, center, up);
		}

		~Camera()
		{ }

		// Update camera
		void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
		{
			this->position = position;
			forward = glm::normalize(center - position);
			right = glm::cross(forward, up);
			this->up = glm::cross(right, forward);
		}

		void MoveForward(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, forward.y, 0));
			// movement will keep the camera at the same height always
			// Example: If you rotate up/down your head and walk forward you will still keep the same relative distance (height) to the ground!
			// Translate the camera using the DIR vector computed from forward
		}

		void TranslateForward(float distance)
		{
			// TODO : Translate the camera using the "forward" vector
			position = position + glm::normalize(forward) * distance;
		}

		void TranslateUpward(float distance)
		{
			// TODO : Translate the camera using the up vector
			position = position + glm::normalize(up) * distance;
		}

		void TranslateRight(float distance)
		{
			// TODO
			// Translate the camera using the "right" vector
			// Usually translation using camera "right' is not very useful because if the camera is rotated around the "forward" vector 
			// translation over the right direction will have an undesired effect; the camera will get closer or farther from the ground
			// Using the projected right vector (onto the ground plane) makes more sense because we will keep the same distance from the ground plane
			position = position + glm::normalize(right) * distance;
		}

		void RotateFirstPerson_OX(float angle)
		{
			// TODO
			// Compute the new "forward" and "up" vectors
			// Attention! Don't forget to normalize the vectors
			// Use glm::rotate()
			glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 0);

			forward = glm::normalize(glm::vec3(newVector));

			up = glm::cross(right, forward);
		}

		void RotateFirstPerson_OY(float angle)
		{
			// TODO
			// Compute the new "forward", "up" and "right" vectors
			// Don't forget to normalize the vectors
			// Use glm::rotate()
			glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);

			forward = glm::normalize(glm::vec3(newVector));


			newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(up, 0);

			up = glm::normalize(glm::vec3(newVector));

			newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);

			right = glm::normalize(glm::vec3(newVector));
		}

		void RotateFirstPerson_OZ(float angle)
		{
			// TODO
			// Compute the new Right and Up, Forward stays the same
			// Don't forget to normalize the vectors

			glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::vec4(up, 0);

			up = glm::normalize(glm::vec3(newVector));

			newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1)) * glm::vec4(right, 0);

			right = glm::normalize(glm::vec3(newVector));
		}

		void RotateThirdPerson_OX(float angle)
		{
			// TODO
			// Rotate the camera in Third Person mode - OX axis
			// Use distanceToTarget as translation distance
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OX(angle);
			TranslateForward(-distanceToTarget);
		}

		void RotateThirdPerson_OY(float angle)
		{
			// TODO
			// Rotate the camera in Third Person mode - OY axis
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OY(angle);
			TranslateForward(-distanceToTarget);
		}

		void RotateThirdPerson_OZ(float angle)
		{
			// TODO
			// Rotate the camera in Third Person mode - OZ axis
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OZ(angle);
			TranslateForward(-distanceToTarget);
		}

		glm::mat4 GetViewMatrix()
		{
			// Returns the View Matrix
			return glm::lookAt(position, position + forward, up);
		}

		glm::vec3 GetTargetPosition()
		{
			return position + forward * distanceToTarget;
		}

	public:
		float distanceToTarget;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};

}
