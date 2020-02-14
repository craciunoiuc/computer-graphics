#pragma once

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <GL/glew.h>

namespace Objects
{
	Mesh* CreateCube(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateCylinder(std::string name, glm::vec3 leftBottomCorner, float radius, float height, glm::vec3 color);
	Mesh* CreateTriangle2D(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateSquare2D(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateSphere(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color);
	Mesh* CreateTetrahedron(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreatePlane(std::string name, glm::vec3 leftBottomCorner, float unitLength, glm::vec3 color, unsigned char* test);
}