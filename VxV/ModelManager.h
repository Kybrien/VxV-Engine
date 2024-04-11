#pragma once
#include "Manager.h"
#include "Model.h"

class ModelManager : public Manager
{
private:
	std::vector<ModelComponent*> modelVector;
	std::vector<std::vector<std::string>> allModelProperties;

	static std::string fileDirection;
	static std::string extention;
public:
	ModelManager(Manager* manager);

	std::vector<ModelComponent*> GetModels() {
		return modelVector;
	}

	std::vector<std::vector<std::string>> GetAllModelProperties() {
		return allModelProperties;
	}

	void AddModel(ModelComponent* mesh) {
		modelVector.push_back(mesh);
	}

	ModelComponent* GetModel(std::string name) {
		for (ModelComponent* mesh : modelVector) {
			if (mesh->fileName == name) {
				return mesh;
			}
		}
		return nullptr;
	}

	void SetModel(std::string modelName, GameObject* go);

	void Load(std::wstring wFileDirection, std::wstring wFileName) override;
	void LoadModel(std::string fileName);
};

