#pragma once
#include "Manager.h"
#include "Mesh.h"

class MeshManager : public Manager
{
private:
	std::vector<Object*> meshVector;
	std::vector<std::vector<std::string>> allMeshProperties;

	static std::string fileDirection;
	static std::string extention;
public:
	MeshManager(Manager* manager);

	std::vector<Object*> GetMeshs() {
		return meshVector;
	}

	std::vector<std::vector<std::string>> GetAllMeshProperties() {
		return allMeshProperties;
	}

	void AddMesh(Object* mesh) {
		meshVector.push_back(mesh);
	}

	Object* GetMesh(std::string name) {
		for (Object* mesh : meshVector) {
			if (mesh->fileName == name) {
				return mesh;
			}
		}
		return nullptr;
	}

	void SetMesh(std::string meshName, GameObject* go);

	void Load(std::wstring wFileDirection, std::wstring wFileName) override;
	void LoadMesh(std::string fileName);
};

