#include <vector>
#include <iostream>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/Tema2/Object_3D.h"

using namespace std;


// Track base points
std::vector<VertexFormat> aux_vert = {
			VertexFormat(glm::vec3(14, 0,  0)), // 0
			VertexFormat(glm::vec3(12, 0,  4)), // 1
			VertexFormat(glm::vec3(10, 0,  8)), // 2
			VertexFormat(glm::vec3(5, 0,  10)), // 3
			VertexFormat(glm::vec3(0, 0,  12)), // 4
			VertexFormat(glm::vec3(-4, 0, 10)), // 5
			VertexFormat(glm::vec3(-7.5, 0,  8.2)), // 6
			VertexFormat(glm::vec3(-12, 0, 9)), // 7
			VertexFormat(glm::vec3(-16, 0,  6.8)), // 8
			VertexFormat(glm::vec3(-19, 0,  4.5)), // 9
			VertexFormat(glm::vec3(-20, 0,  0)), // 10
			VertexFormat(glm::vec3(-17, 0,  -4)), // 11
			VertexFormat(glm::vec3(-14.9, 0,  -7)), // 12
			VertexFormat(glm::vec3(-10, 0, -8)), // 13
			VertexFormat(glm::vec3(-5.2, 0, -7.2)), // 14
			VertexFormat(glm::vec3(0, 0,  -5)), // 15 
			VertexFormat(glm::vec3(4.3, 0,  -6.6)), // 16
			VertexFormat(glm::vec3(7.6, 0,  -7.9)), // 17
			VertexFormat(glm::vec3(11.1, 0,  -7.3)), // 18
			VertexFormat(glm::vec3(13.1, 0,  -3.6)), // 19
};


/* Creates race track */
Mesh* objects_3D::CreateRaceTrack(const std::string& name, glm::vec3 color)
{	
	
	// Determine track limmits
	std::vector < VertexFormat> vertices;
	glm::vec3 dist = glm::vec3(0.3, 0, 0.3);
	glm::vec3 D, P, R, A;

	for (int i = 0; i < aux_vert.size() - 1; i++) {
		D = aux_vert[i + 1].position - aux_vert[i].position;
		P = cross(D, glm::vec3(0, 1, 0));
		R = aux_vert[i].position + dist * P;
		A = aux_vert[i].position - dist * P;
		vertices.push_back(VertexFormat(R,color));
		vertices.push_back(VertexFormat(A, color));
	}

	D = aux_vert[0].position - aux_vert[aux_vert.size() - 1].position;
	P = cross(D, glm::vec3(0, 1, 0));
	R = aux_vert[aux_vert.size() - 1].position + dist * P;
	A = aux_vert[aux_vert.size() - 1].position - dist * P;
	vertices.push_back(VertexFormat(R,color));
	vertices.push_back(VertexFormat(A,color));
	
	// Adds triangles between track limits
	std::vector<unsigned int> indices;
	for (int i = 0; i < vertices.size() - 2; i++) {

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);

	}

	indices.push_back(vertices.size() - 2);
	indices.push_back(vertices.size() - 1);
	indices.push_back(0);

	indices.push_back(vertices.size() - 1);
	indices.push_back(0);
	indices.push_back(1);

	Mesh* raceTrack = new Mesh(name);
	raceTrack->InitFromData(vertices, indices);
	return raceTrack;
}

/* Creates a tree */
Mesh* objects_3D::CreateTrees(const std::string& name, glm::vec3 color_base, glm::vec3 color_leafs)
{

	vector<VertexFormat> tree_vertices
	{
		VertexFormat(glm::vec3(-0.15, 0,  0.15), color_base),
		VertexFormat(glm::vec3(0.15, 0,  0.15), color_base),
		VertexFormat(glm::vec3(-0.15, 0.8,  0.15), color_base),
		VertexFormat(glm::vec3(0.15, 0.8,  0.15), color_base),
		VertexFormat(glm::vec3(-0.15, 0, -0.15), color_base),
		VertexFormat(glm::vec3(0.15, 0,  -0.15), color_base),
		VertexFormat(glm::vec3(-0.15, 0.8,  -0.15), color_base),
		VertexFormat(glm::vec3(0.15, 0.8,  -0.15), color_base),
	};	

	tree_vertices.push_back(VertexFormat(glm::vec3(-0.5, 0.8, 0.5), color_leafs));
	tree_vertices.push_back(VertexFormat(glm::vec3(0.5, 0.8, 0.5), color_leafs));
	tree_vertices.push_back(VertexFormat(glm::vec3(-0.5, 1.8, 0.5), color_leafs));
	tree_vertices.push_back(VertexFormat(glm::vec3(0.5, 1.8, 0.5), color_leafs));
	tree_vertices.push_back(VertexFormat(glm::vec3(-0.5, 0.8, -0.5), color_leafs));
	tree_vertices.push_back(VertexFormat(glm::vec3(0.5, 0.8, -0.5), color_leafs));
	tree_vertices.push_back(VertexFormat(glm::vec3(-0.5, 1.8, -0.5), color_leafs));
	tree_vertices.push_back(VertexFormat(glm::vec3(0.5, 1.8, -0.5), color_leafs));

	vector<unsigned int> indices =
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

	for (int i = 0; i < 36; i++) {
		indices.push_back(indices[i] + 8);
	}


	Mesh* trees = new Mesh(name);
	trees->InitFromData(tree_vertices, indices);
	return trees;
}

/* Creates the race car */
Mesh* objects_3D::CreateCar(const std::string& name, glm::vec3 color)
{
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, 0,  0.6), color),
		VertexFormat(glm::vec3(0.3, 0,  0.6), color),
		VertexFormat(glm::vec3(0, 0.3,  0.6), color),
		VertexFormat(glm::vec3(0.3, 0.3,  0.6), color),
		VertexFormat(glm::vec3(0, 0,  0), color),
		VertexFormat(glm::vec3(0.3, 0,  0), color),
		VertexFormat(glm::vec3(0, 0.3,  0), color),
		VertexFormat(glm::vec3(0.3, 0.3,  0), color),
	};

	vector<unsigned int> indices =
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

	Mesh* car = new Mesh(name);
	car->InitFromData(vertices, indices);
	return car;
}

/* Makes the ground green */
Mesh* objects_3D::CreateGrass(const std::string& name, glm::vec3 color) {
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-25, -0.01,  25), color),
		VertexFormat(glm::vec3(25, -0.01,  25), color),
		VertexFormat(glm::vec3(-25, -0.01,  -25), color),
		VertexFormat(glm::vec3(25, -0.01,  -25), color),
	};
	vector<unsigned int> indices =
	{
		0,1,2,
		1,3,2
	};
	Mesh* grass = new Mesh(name);
	grass->InitFromData(vertices, indices);
	return grass;
}
