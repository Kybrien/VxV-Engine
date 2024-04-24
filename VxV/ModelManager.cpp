#include "ModelManager.h"
#include "GameObject.h"
#include "Debug.h"


std::string ModelManager::fileDirection = "Files";
std::string ModelManager::extention = ".obj";

ModelManager::ModelManager(Manager* manager) {
	manager->AddManager<ModelManager>(this);

	SearchFile<ModelManager>(fileDirection, extention, this);

    LoadModel("cube.obj");
	modelVector[0]->numberUsed++;
}


void ModelManager::Load(std::wstring wFileDirection, std::wstring wFileName) {
    // Conversion de wstring en string
    std::string fileDirection;
    for (wchar_t wc : wFileDirection) {
        fileDirection.push_back(static_cast<char>(wc));
    }

    // Conversion de wstring en string
    std::string fileName;
    for (wchar_t wc : wFileName) {
        fileName.push_back(static_cast<char>(wc));
    }

    std::vector<std::string> temp;
    temp.push_back(fileDirection);
    temp.push_back(fileName);

    allModelProperties.push_back(temp);
}


void ModelManager::LoadModel(std::string fileName) {

    bool foundMesh = false;
    for (std::vector<std::string> strVec : allModelProperties) {
        if (strVec[1] == fileName) {
            addNewModel(strVec[1], strVec[0], modelVector);
            foundMesh = true;
            break;
        }
    }

    if (!foundMesh) {
		Debug::Log("Mesh not found");
	}
}


void ModelManager::SetModel(std::string modelName, GameObject* go) {
	ModelComponent* modelToAssign = GetModel(modelName);
	ModelComponent* modelGO = go->GetComponent<Model>()->GetModel();

	// Vérifier si c'est loadé
	if (modelToAssign != nullptr) {
		ModelComponent* tempModel = modelGO;

		modelToAssign->numberUsed++;
		if(modelGO != nullptr)
			modelGO->numberUsed--;
		
		go->GetComponent<Model>()->GetModel() = modelToAssign;

		// Supprimer l'élément s'il n'est plus utilisé
		if (tempModel->numberUsed == 0) {
			auto it = std::find(modelVector.begin(), modelVector.end(), tempModel);
			if (it != modelVector.end()) {
				modelVector.erase(it);
			}
			delete tempModel;
		}
	}
	else {
		bool modelFound = false;
		for (std::vector<std::string> strVec : allModelProperties) {
			if (strVec[1] == modelName) {
				addNewModel(strVec[1], strVec[0], modelVector);
				SetModel(modelName, go);
				modelFound = true;
				break;
			}
		}

		if (modelFound == false)
			Debug::Log("Model " + modelName + " not found");
	}
}