#pragma once
#include <GLM/glm.hpp>

#include "Component.h"

using namespace glm;

class Transform : public Component {
public:
	Transform(GameObject * gameObject) : Component(gameObject) {

	}


	void Load() override {

	}

	void Save() override {

	}

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;


	
};