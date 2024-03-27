#pragma once
#include "Component.h"
#include <glm/glm.hpp>

using namespace glm;
class Script : public Component {
public:
	std::string name;
	Script(GameObject* gameObject) : Component(gameObject) {
		type = Component::Type::Script;
	}
	void Load(Json::Value& compJson) override {
	}

	void Save(Json::Value& compJson) override {
	}
};
