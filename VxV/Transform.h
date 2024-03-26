#pragma once
#include "Component.h"
#include <glm/glm.hpp>

using namespace glm;
class Transform : public Component {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;



	Transform(GameObject* gameObject) : Component(gameObject) {

		name = "Transform";
	}


	void Load() override {

	}

	void Save(Json::Value& compJson) override {
		compJson["Name"] = name;

		compJson["Position"] = Json::Value(Json::arrayValue);
		SaveVec3(compJson["Position"], position);

		compJson["Rotation"] = Json::Value(Json::arrayValue);
		SaveVec3(compJson["Rotation"], rotation);

		compJson["Scale"] = Json::Value(Json::arrayValue);
		SaveVec3(compJson["Scale"], scale);
	}


private:
	void SaveVec3(Json::Value& compJson, glm::vec3 vec) {
		compJson.append(vec.x);
		compJson.append(vec.y);
		compJson.append(vec.z);
	}
};
