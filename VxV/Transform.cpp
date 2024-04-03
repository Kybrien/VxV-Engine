#include "Transform.h"



Transform::Transform(GameObject* gameObject) : Component(gameObject) {
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(0, 0, 0);

	type = Component::Type::Transform;
	linkedGameObject = gameObject;
}


void Transform::Load(Json::Value& compJson, GameObject* ParentGO) {

	LoadVec3(compJson["Position"], position);
	LoadVec3(compJson["Rotation"], rotation);
	LoadVec3(compJson["Scale"], scale);

	linkedGameObject = ParentGO;
}

void Transform::Save(Json::Value& compJson) {
	compJson["Type"] = type;

	compJson["Position"] = Json::Value(Json::arrayValue);
	SaveVec3(compJson["Position"], position);

	compJson["Rotation"] = Json::Value(Json::arrayValue);
	SaveVec3(compJson["Rotation"], rotation);

	compJson["Scale"] = Json::Value(Json::arrayValue);
	SaveVec3(compJson["Scale"], scale);
}
















void Transform::SaveVec3(Json::Value& compJson, glm::vec3 vec) {
	compJson.append(vec.x);
	compJson.append(vec.y);
	compJson.append(vec.z);
}

void Transform::LoadVec3(Json::Value& compJson, glm::vec3& vec) {
	vec.x = compJson[0].asFloat();
	vec.y = compJson[1].asFloat();
	vec.z = compJson[2].asFloat();
}

