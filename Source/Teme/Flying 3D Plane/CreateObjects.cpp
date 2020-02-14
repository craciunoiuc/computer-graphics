#include "CreateObjects.h"

// Creates a Cube
Mesh* Objects::CreateCube(std::string name, glm::vec3 leftBottomCorner, 
						  float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, 0,  length), color),
		VertexFormat(corner + glm::vec3(length, 0,  length), color),
		VertexFormat(corner + glm::vec3(0, length,  length), color),
		VertexFormat(corner + glm::vec3(length, length,  length), color),
		VertexFormat(corner + glm::vec3(0, 0,  0), color),
		VertexFormat(corner + glm::vec3(length, 0,  0), color),
		VertexFormat(corner + glm::vec3(0, length,  0), color),
		VertexFormat(corner + glm::vec3(length, length,  0), color)
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		1, 3, 2,
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,
	};

	Mesh* cube = new Mesh(name);
	cube->InitFromData(vertices, indices);
	return cube;
}

// Creates a square for the user interface
Mesh* Objects::CreateSquare2D(std::string name, glm::vec3 leftBottomCorner,
							  float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

	square->InitFromData(vertices, indices);
	return square;
}

// Creates a triangle for the user interface
Mesh* Objects::CreateTriangle2D(std::string name, glm::vec3 leftBottomCorner, 
								float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length / 2, length, 0), color)
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };

	triangle->InitFromData(vertices, indices);
	return triangle;
}

// Creates a tube
Mesh* Objects::CreateCylinder(std::string name, glm::vec3 leftBottomCorner,
								float radius, float height, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;
	float coordOX = 0.0f;
	float coordOY = 0.0f;
	float angle = 0.0f;
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	int ind = 0;
	const float precision = 0.01f;
	for (float end = 0.5f; end <= height; end += 0.5f) {
		angle = 0.0f;
		while (angle < 6.28318530718f) {
			coordOX = radius * cos(angle);
			coordOY = radius * sin(angle);
			vertices.push_back(VertexFormat(corner + glm::vec3(coordOX, coordOY, end), color));
			vertices.push_back(VertexFormat(corner + glm::vec3(coordOX, coordOY, end - 0.5f), color));
			if (ind % 2 != 1) {
				indices.push_back(ind);
				indices.push_back(ind + 1);
				indices.push_back(ind + 2);
			}
			else {
				indices.push_back(ind);
				indices.push_back(ind + 2);
				indices.push_back(ind + 1);
			}
			ind++;
			if (ind % 2 != 1) {
				indices.push_back(ind);
				indices.push_back(ind + 1);
				indices.push_back(ind + 2);
			}
			else {
				indices.push_back(ind);
				indices.push_back(ind + 2);
				indices.push_back(ind + 1);
			}
			ind++;
			angle += precision;
		}

		vertices.push_back(VertexFormat(corner + glm::vec3(radius, 0, end), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(radius, 0, end - 0.5f), color));

		angle = 0.0f;
		while (angle < 6.28318530718f) {
			coordOX = radius * cos(angle);
			coordOY = radius * sin(angle);
			vertices.push_back(VertexFormat(corner + glm::vec3(coordOX, coordOY, end), color));
			if (ind % 2 != 1) {
				indices.push_back(ind);
				indices.push_back(ind + 1);
				indices.push_back(ind + 2);
			}
			else {
				indices.push_back(ind);
				indices.push_back(ind + 2);
				indices.push_back(ind + 1);
			}
			ind++;
			angle += precision;
		}
		vertices.push_back(VertexFormat(corner + glm::vec3(radius, 0, end), color));

	}
	Mesh* cylinder = new Mesh(name);
	cylinder->InitFromData(vertices, indices);
	return cylinder;
}

// Creates a simple tetrahedron used for fuel
Mesh* Objects::CreateTetrahedron(std::string name, glm::vec3 leftBottomCorner, 
								 float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices = 
	{
		VertexFormat(corner + glm::vec3(length * 0.5f, length * 1, length * 0.5f), color),
		VertexFormat(corner + glm::vec3(length * 0, length * 0, length * 0), color),
		VertexFormat(corner + glm::vec3(length * 1, length * 0, length * 0), color),

		VertexFormat(corner + glm::vec3(length * 0.5f, length * 1, length * 0.5f), color),
		VertexFormat(corner + glm::vec3(length * 1, length * 0, length * 0), color),
		VertexFormat(corner + glm::vec3(length * 0.5f, length * 0, length * 1), color),

		VertexFormat(corner + glm::vec3(length * 0.5f, length * 1, length * 0.5f), color),
		VertexFormat(corner + glm::vec3(length * 0, length * 0, length * 0), color),
		VertexFormat(corner + glm::vec3(length * 0.5f, length * 0, length * 1), color),


		VertexFormat(corner + glm::vec3(length * 0, length * 0, length * 0), color),
		VertexFormat(corner + glm::vec3(length * 1, length * 0, length * 0), color),
		VertexFormat(corner + glm::vec3(length * 0.5f, length * 0, length * 1), color)

	};
	std::vector<unsigned short> indices = 
	{
		0, 2, 1,
		3, 5, 4,
		6, 7, 8,
		9, 10, 11
	};

	Mesh* tetrahedron = new Mesh(name);
	tetrahedron->InitFromData(vertices, indices);
	return tetrahedron;
}

//Creates a sphere used for obstacles
Mesh* Objects::CreateSphere(std::string name, glm::vec3 leftBottomCorner, 
							float radius, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	float PI = 3.14159f;
	int resolution = 5;
	int ind = 0;
	for (float theta = 0.0; theta < PI; theta += PI / resolution) {
		for (float phi = 0.0; phi < 2 * PI; phi += PI / resolution) {
			vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(phi) * sin(theta), 
								radius * sin(phi) * sin(theta), radius * cos(theta)), color));
			indices.push_back(ind); indices.push_back(ind + 1); indices.push_back(ind + 2);
			ind++;

			vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(phi) * sin(theta + PI / resolution), 
				radius * sin(phi) * sin(theta + PI / resolution), radius * cos(theta + PI / resolution)), color));
			indices.push_back(ind); indices.push_back(ind + 1); indices.push_back(ind + 2);
			ind++;
		}
	}

	Mesh* sphere = new Mesh(name);
	sphere->SetDrawMode(GL_TRIANGLE_STRIP);
	sphere->InitFromData(vertices, indices);
	return sphere;
}
