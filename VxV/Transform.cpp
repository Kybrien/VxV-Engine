#include "Transform.h"
#include "GameObject.h"

#include "ModelComponent.hpp"
#include "Model.h"

#include "Debug.h"

#include "Engine.h"



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

void Transform::Copy(GameObject* goToFill) {

	Transform* newTrans = goToFill->GetComponent<Transform>();
	newTrans->position = glm::vec3(position.x, position.y, position.z);
	newTrans->rotation = glm::vec3(rotation.x, rotation.y, rotation.z);
	newTrans->scale = glm::vec3(scale.x, scale.y, scale.z);
}


void Transform::SetPosition(glm::vec3 pos) {
	position = pos;
	setTranslationModel(*linkedGameObject->GetComponent<Model>()->GetModel(), pos);
}


void Transform::SetRotation(float xAngle, float yAngle, float zAngle) {

	rotation = glm::vec3(xAngle, yAngle, zAngle);
	if (xAngle != 0)
		setRotationModel(*linkedGameObject->GetComponent<Model>()->GetModel(), xAngle, glm::vec3(1, 0, 0));
	if (yAngle != 0)
		setRotationModel(*linkedGameObject->GetComponent<Model>()->GetModel(), yAngle, glm::vec3(0, 1, 0));
	if (zAngle != 0)
		setRotationModel(*linkedGameObject->GetComponent<Model>()->GetModel(), zAngle, glm::vec3(0, 0, 1));
}

void Transform::SetScale(glm::vec3 sca) {
	scale = sca;
	setScaleModel(*linkedGameObject->GetComponent<Model>()->GetModel(), sca);
}


void Transform::Translate(glm::vec3 pos) {
	position += pos;
	translateModel(*linkedGameObject->GetComponent<Model>()->GetModel(), position);
}

void Transform::Rotate(float xAngle, float yAngle, float zAngle) {
	rotation += glm::vec3(xAngle, yAngle, zAngle);
	if (xAngle != 0)
		rotateModel(*linkedGameObject->GetComponent<Model>()->GetModel(), xAngle, glm::vec3(1, 0, 0));
	if (yAngle != 0)
		rotateModel(*linkedGameObject->GetComponent<Model>()->GetModel(), yAngle, glm::vec3(0, 1, 0));
	if (zAngle != 0)
		rotateModel(*linkedGameObject->GetComponent<Model>()->GetModel(), zAngle, glm::vec3(0, 0, 1));
}

void Transform::Scale(glm::vec3 sca) {
	scale += sca;
	scaleModel(*linkedGameObject->GetComponent<Model>()->GetModel(), scale);
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

