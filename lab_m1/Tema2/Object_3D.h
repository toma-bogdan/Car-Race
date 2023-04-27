#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include <vector>

using namespace std;


namespace objects_3D
{
	Mesh* CreateRaceTrack(const std::string& name, glm::vec3 color);
	Mesh* CreateTrees(const std::string& name, glm::vec3 color_base, glm::vec3 color_leafs);
	Mesh* CreateCar(const std::string& name, glm::vec3 color);
	Mesh* CreateGrass(const std::string& name, glm::vec3 color);
}