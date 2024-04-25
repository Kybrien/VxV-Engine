#pragma once
#include "Component.h"

class Light : public Component
{
public:
	/**
	* @brief Instanciate a component Light.
	* @param A reference to the linked GameObject
	*/
	Light(GameObject* gameObject);

	void Save(Json::Value& compJson) override;

	void Copy(GameObject* goToFill) override;
};

