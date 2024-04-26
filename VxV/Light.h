#pragma once
#include "Component.h"
#include "glm/glm.hpp"
class Light : public Component
{
public:

	float power = 80.0f;
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	/**
	* @brief Instanciate a component Light.
	* @param A reference to the linked GameObject
	*/
	Light(GameObject* gameObject);


	void Load(Json::Value& compJson, GameObject* ParentGO) override {}

	void Save(Json::Value& compJson) override;

	void Copy(GameObject* goToFill) override;

	void SetColor(glm::vec3 _color);

	glm::vec3 GetColor();

	void SetPower(float _power);

	float GetPower();

};

