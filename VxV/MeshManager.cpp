#include "MeshManager.h"
#include "GameObject.h"
#include "Debug.h"


std::string MeshManager::fileDirection = "Files";
std::string MeshManager::extention = ".obj";

MeshManager::MeshManager(Manager* manager) {
	manager->AddManager<MeshManager>(this);

	SearchFile<MeshManager>(fileDirection, extention, this);

    LoadMesh("cube.obj");
}


void MeshManager::Load(std::wstring wFileDirection, std::wstring wFileName) {
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

    allMeshProperties.push_back(temp);
}


void MeshManager::LoadMesh(std::string fileName) {

    bool foundMesh = false;
    for (std::vector<std::string> strVec : allMeshProperties) {
        if (strVec[1] == fileName) {
            addNewObject(strVec[1], strVec[0], meshVector);
            foundMesh = true;
            break;
        }
    }

    if (!foundMesh) {
		Debug::Log("Mesh not found");
	}
}


void MeshManager::SetMesh(std::string meshName, GameObject* go) {
	Object* meshToAssign = GetMesh(meshName);
	Object* meshGO = go->GetComponent<Mesh>()->GetMesh();

	// Vérifier si c'est loadé
	if (meshToAssign != nullptr) {
		Object* tempMesh = meshGO;

		meshToAssign->numberUsed++;
		if(meshGO != nullptr)
			meshGO->numberUsed--;
		
		go->GetComponent<Mesh>()->GetMesh() = meshToAssign;

		// Supprimer l'élément s'il n'est plus utilisé
		if (tempMesh->numberUsed == 0) {
			auto it = std::find(meshVector.begin(), meshVector.end(), tempMesh);
			if (it != meshVector.end()) {
				meshVector.erase(it);
			}
			delete tempMesh;
		}
	}
	else {
		bool meshFound = false;
		for (std::vector<std::string> strVec : allMeshProperties) {
			if (strVec[1] == meshName) {
				addNewObject(strVec[1], strVec[0], meshVector);
				SetMesh(meshName, go);
				meshFound = true;
				break;
			}
		}

		if (meshFound == false)
			Debug::Log("Mesh " + meshName + " not found");
	}
}