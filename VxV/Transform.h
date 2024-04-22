#pragma once
#include <glm/glm.hpp>
#include "Component.h"


using namespace glm;
class Transform : public Component {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;



	Transform(GameObject* gameObject);

	void SetPosition(glm::vec3 pos);
	void SetRotation(float rot, glm::vec3 axis);
	void SetScale(glm::vec3 sca);

	void Translate(glm::vec3 pos);
	void Rotate(float rot, glm::vec3 axis);
	void Scale(glm::vec3 sca);


	void Load(Json::Value& compJson, GameObject* ParentGO) override;

	void Save(Json::Value& compJson) override;

	void Copy(GameObject* goToFill) override;

private:
	void SaveVec3(Json::Value& compJson, glm::vec3 vec);

	void LoadVec3(Json::Value& compJson, glm::vec3& vec);
};
