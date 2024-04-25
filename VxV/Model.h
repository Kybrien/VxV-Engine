#pragma once


#include "Component.h"
#include "ModelComponent.hpp"


class Model : public Component
{
private:
	/**
	* @brief The model assigned to the component
	*/
	ModelComponent* model = nullptr;

public:
	/**
	* @brief Instanciate a component Model.
	* @param A reference to the linked GameObject
	*/
	Model(GameObject* go);


	void Save(Json::Value& compJson) override;

	void Load(Json::Value& compJson, GameObject* ParentGO) override;

	void Copy(GameObject* goToFill) override;

	/**
	* @brief Assign the 3d model to the component, searching the name in the model manager
	* @param A string containing the name of the model
	* @return void
	*/
	void SetModel(std::string modelName);

	/**
	* @brief Get the model assigned to the component
	* @return ModelComponent*&
	*/
	ModelComponent*& GetModel() {
		return model;
	}
};
