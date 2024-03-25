#pragma once
#include "Component.h"
#include <glm/glm.hpp>
using namespace glm;
class Transform : public Component {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

	//Transform(GameObject* gameObject) : Component(gameObject) {
	//
	//}

	void Load() override {
	
	}
	
	void Save() override {
	
	}
};
