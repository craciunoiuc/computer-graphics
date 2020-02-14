#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateCircle(std::string name, glm::vec3 center, float length, glm::vec3 color, bool fill = false);
	bool  ObjectsOverlap(std::pair<std::pair<int, int>, std::pair<int, int>> obj1, std::pair<std::pair<int, int>, std::pair<int, int>> obj2);
	bool  ObjectsOverlapOX(std::pair<std::pair<int, int>, std::pair<int, int>> obj1, std::pair<std::pair<int, int>, std::pair<int, int>> obj2);
	bool  ObjectsOverlapOY(std::pair<std::pair<int, int>, std::pair<int, int>> obj1, std::pair<std::pair<int, int>, std::pair<int, int>> obj2);
}