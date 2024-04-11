#pragma once


#include "Component.h"
#include "ModelComponent.hpp"


class Model : public Component
{
private:
	ModelComponent* model;

public:
	Model(GameObject* go);

	void Save(Json::Value& compJson) override;
	void Load(Json::Value& compJson, GameObject* ParentGO) override;

	void Copy(GameObject* goToFill) override;

	void SetModel(std::string modelName);

	ModelComponent*& GetModel() {
		return model;
	}
};
