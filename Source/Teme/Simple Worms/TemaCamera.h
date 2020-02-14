#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace Tema
{
	class Camera
	{
		public:
			Camera()
			{
				position = glm::vec3(-0.0136797f, 1.51564f, 7.45895f);
				forward  = glm::vec3(0.0322143f, -0.105355f, -0.993913f);
				right    = glm::vec3(0.999475f, 0, 0.0323943f);
				up   	 = glm::vec3(0.0034129f, 0.994435f, -0.1053f);
				distanceToTarget = 2;
			}

			Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				Set(position, center, up);
			}

			~Camera()
			{ }

			// Sets new values for the camera
			void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				this->position = position;
				forward = glm::normalize(center-position);
				right	= glm::cross(forward, up);
				this->up = glm::cross(right,forward);
			}

			// Moves the camera forward
			void MoveForward(float distance)
			{
				glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
			}

			// Translates the camera forward
			void TranslateForward(float distance)
			{
				position = position + glm::normalize(forward) * distance;
			}

			// Translates the camera upward
			void TranslateUpward(float distance)
			{
				position = position + glm::normalize(up) * distance;
			}

			// Translates the camera right
			void TranslateRight(float distance)
			{
				position = position + glm::normalize(right) * distance;
			}

			// Rotates the camera on the OX axis
			void RotateFirstPerson_OX(float angle)
			{
				glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 0);
				forward = glm::normalize(glm::vec3(newVector));
				up = glm::cross(right, forward);
			}

			// Rotates the camera on the OY axis
			void RotateFirstPerson_OY(float angle)
			{
				glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
				forward = glm::normalize(glm::vec3(newVector));
				newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(up, 0);
				up = glm::normalize(glm::vec3(newVector));
				newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);
				right = glm::normalize(glm::vec3(newVector));
			}

			// Rotates the camera on the OZ axis
			void RotateFirstPerson_OZ(float angle)
			{
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

			// Returns the view matrix
			glm::mat4 GetViewMatrix()
			{
				return glm::lookAt(position, position + forward, up);
			}

			// Returns the target of the camera
			glm::vec3 GetTargetPosition()
			{
				return position + forward * distanceToTarget;
			}

			// Returns the position of the camera
			glm::vec3 getPosition() {
				return position;
			}

		public:
			float distanceToTarget;
			glm::vec3 position;
			glm::vec3 forward;
			glm::vec3 right;
			glm::vec3 up;
			bool FPP = false;
		};
}