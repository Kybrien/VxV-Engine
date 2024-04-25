#pragma once
#include <glm/glm.hpp>
#include "Component.h"


using namespace glm;
class Transform : public Component {
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;


public:

	Transform(GameObject* gameObject);

	void SetPosition(glm::vec3 pos, bool isChild = false);
	void SetRotation(float xAngle, float yAngle, float zAngle, bool isChild = false);
	void SetScale(glm::vec3 sca, bool isChild = false);

	void Translate(glm::vec3 pos, bool isChild = false);
	void Rotate(float xAngle, float yAngle, float zAngle, bool isChild = false);
	void Scale(glm::vec3 sca, bool isChild = false);

	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetRotation() { return rotation; }
	glm::vec3 GetScale() { return scale; }


	void Load(Json::Value& compJson, GameObject* ParentGO) override;

	void Save(Json::Value& compJson) override;

	void Copy(GameObject* goToFill) override;

private:
	void SaveVec3(Json::Value& compJson, glm::vec3 vec);

	void LoadVec3(Json::Value& compJson, glm::vec3& vec);
};
