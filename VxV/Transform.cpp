#include "Engine.hpp"
#include "Transform.h"
#include "GameObject.h"

#include "ModelComponent.hpp"
#include "Model.h"

#include "Debug.h"




Transform::Transform(GameObject* gameObject) : Component(gameObject) {
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);

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


void Transform::SetPosition(glm::vec3 pos, bool isChild) {
	if (!isChild) {
		position = pos;
	}


	glm::vec3 tempPosition = linkedGameObject->origin + pos;
	for (GameObject* child : linkedGameObject->GetChilds())
	{
		child->origin = tempPosition;
	}

	for(GameObject* child : linkedGameObject->GetChilds())
		child->GetComponent<Transform>()->SetPosition(child->GetComponent<Transform>()->position + tempPosition, true);
}


void Transform::SetRotation(float xAngle, float yAngle, float zAngle, bool isChild) {

	rotation = glm::vec3(xAngle, yAngle, zAngle);

	for(GameObject* child : linkedGameObject->GetChilds())
		child->GetComponent<Transform>()->SetRotation(child->GetComponent<Transform>()->rotation.x + xAngle, child->GetComponent<Transform>()->rotation.y + yAngle, child->GetComponent<Transform>()->rotation.z + zAngle, true);
}

void Transform::SetScale(glm::vec3 sca, bool isChild) {
	scale = sca;

	for(GameObject* child : linkedGameObject->GetChilds())
		child->GetComponent<Transform>()->SetScale(child->GetComponent<Transform>()->scale * sca, true);
}


void Transform::Translate(glm::vec3 pos, bool isChild) {
	if (!isChild) {
		position += pos;
	}


	for (GameObject* child : linkedGameObject->GetChilds())
	{
		child->origin = position;
	}

	for(GameObject* child : linkedGameObject->GetChilds())
		child->GetComponent<Transform>()->Translate(pos, true);
}

void Transform::Rotate(float xAngle, float yAngle, float zAngle, bool isChild) {
	rotation += glm::vec3(xAngle, yAngle, zAngle);

	for(GameObject* child : linkedGameObject->GetChilds())
		child->GetComponent<Transform>()->Rotate(xAngle, yAngle, zAngle, true);
}

void Transform::Scale(glm::vec3 sca, bool isChild) {
	scale += sca;

	for(GameObject* child : linkedGameObject->GetChilds())
		child->GetComponent<Transform>()->Scale(sca, true);
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

