#pragma once
#include <GLM/glm.hpp>

#include "Component.h"

using namespace glm;

class Transform : Component {
public:
	Transform(GameObject * gameObject) : Component(gameObject) {

	}


	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;


	
};