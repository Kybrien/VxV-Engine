#pragma once
#include "Manager.h"
#include "Mesh.h"

class MeshManager : public Manager
{
private:
	std::vector<Object> meshVector;

	static std::string fileDirection;
	static std::string extention;
public:
	MeshManager(Manager* manager);

	std::vector<Object> GetMeshs() {
		return meshVector;
	}

	void AddMesh(Object mesh) {
		meshVector.push_back(mesh);
	}

	void Load(std::wstring wFileDirection, std::wstring wFileName) override;
};

