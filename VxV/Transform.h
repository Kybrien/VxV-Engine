#pragma once
#include <glm/glm.hpp>

#include "Component.h"

using namespace glm;
class Transform : public Component {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;



	Transform(GameObject* gameObject) : Component(gameObject) {
		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(0, 0, 0);
		type = Component::Type::Transform;
	}


	void Load(Json::Value& compJson, GameObject* ParentGO) override {

		LoadVec3(compJson["Position"], position);
		LoadVec3(compJson["Rotation"], rotation);
		LoadVec3(compJson["Scale"], scale);

		linkedGameObject = ParentGO;
	}

	void Save(Json::Value& compJson) override {
		compJson["Type"] = type;

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

	void LoadVec3(Json::Value& compJson, glm::vec3& vec) {
		vec.x = compJson[0].asFloat();
		vec.y = compJson[1].asFloat();
		vec.z = compJson[2].asFloat();
	}
};
