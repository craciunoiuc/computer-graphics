#include "Objects.h"
#define GL_LINE_LOOP 0x0002

Mesh* Objects::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	fill = true;
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Objects::CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	fill = true;
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length/2, length, 0), color)
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Objects::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	fill = true;
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, 2 * length, 0), color),
		VertexFormat(corner + glm::vec3(0, 2 * length, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

// <<X, Y>, <Sx, Sy>>
bool Objects::ObjectsOverlap(std::pair<std::pair<int, int>, std::pair<int, int>> obj1,
							 std::pair<std::pair<int, int>, std::pair<int, int>> obj2) {
	bool collisionX = obj1.first.first + obj1.second.first >= obj2.first.first &&
		obj2.first.first + obj2.second.first >= obj1.first.first;
	// Collision y-axis?
	bool collisionY = obj1.first.second + obj1.second.second >= obj2.first.second &&
		obj2.first.second + obj2.second.second >= obj1.first.second;
	// Collision only if on both axes
	return collisionX && collisionY;
	
}

bool Objects::ObjectsOverlapOX(std::pair<std::pair<int, int>, std::pair<int, int>> obj1,
								std::pair<std::pair<int, int>, std::pair<int, int>> obj2) {
	bool collisionX = obj1.first.first + obj1.second.first >= obj2.first.first &&
		obj2.first.first + obj2.second.first >= obj1.first.first;
	// Collision only if on both axes
	return collisionX;

}

bool Objects::ObjectsOverlapOY(std::pair<std::pair<int, int>, std::pair<int, int>> obj1,
								std::pair<std::pair<int, int>, std::pair<int, int>> obj2) {
	bool collisionY = obj1.first.second + obj1.second.second >= obj2.first.second &&
		obj2.first.second + obj2.second.second >= obj1.first.second;
	// Collision only if on both axes
	return collisionY;

}
