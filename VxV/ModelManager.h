#pragma once
#include "Manager.h"
#include "Model.h"

class ModelManager : public Manager
{
private:
	/**
	* @brief Vector containing pointers to ModelComponent objects.
	*/
	std::vector<ModelComponent*> modelVector;

	/**
	* @brief Vector containing vectors of strings representing properties of all models.
	*/
	std::vector<std::vector<std::string>> allModelProperties;

	/**
	* @brief Static string representing the file direction.
	* Used to store the directory where model files are located.
	*/
	static std::string fileDirection;

	/**
	* @brief Static string representing the file extension.
	* Used to specify the file extension for model files.
	*/
	static std::string extention;

public:

	/**
	* @brief Construct a ModelManager instance.
	* @param manager A pointer to the Manager instance managing this ModelManager.
	*/
	ModelManager(Manager* manager);


	/**
	* @brief Retrieve a vector of ModelComponent pointers.
	* @return A vector containing pointers to ModelComponent instances.
	*/
	std::vector<ModelComponent*> GetModels() {
		return modelVector;
	}

	/**
	* @brief Retrieves all model properties.
	* @return A vector of vectors containing the properties of all models.
	*/
	std::vector<std::vector<std::string>> GetAllModelProperties() {
		return allModelProperties;
	}

	/**
	* @brief Adds a model to the collection.
	* @param ModelComponent* Pointer to the ModelComponent to be added.
	* @return void
	*/
	void AddModel(ModelComponent* mesh) {
		modelVector.push_back(mesh);
	}

	/**
	* @brief Retrieves a model by its name.
	* @param std::string The name of the model to retrieve.
	* @return Pointer to the ModelComponent if found, otherwise nullptr.
	*/
	ModelComponent* GetModel(std::string name) {
		for (ModelComponent* mesh : modelVector) {
			if (mesh->fileName == name) {
				return mesh;
			}
		}
		return nullptr;
	}

	/**
	* @brief Sets the model of a GameObject.
	* @param std::string The name of the model to set.
	* @param GameObject* Pointer to the GameObject.
	* @return void
	*/
	void SetModel(std::string modelName, GameObject* go);


	void Load(std::wstring wFileDirection, std::wstring wFileName) override;

	/**
	* @brief Loads a model from a file.
	* @param std::string The name of the file to load the model from.
	* @return void
	*/
	void LoadModel(std::string fileName);

};

