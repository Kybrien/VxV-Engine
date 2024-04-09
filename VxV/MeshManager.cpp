#include "MeshManager.h"


std::string MeshManager::fileDirection = "Mesh";
std::string MeshManager::extention = ".obj";

MeshManager::MeshManager(Manager* manager) {
	manager->AddManager<MeshManager>(this);

	SearchFile<MeshManager>(fileDirection, extention, this);
}

void MeshManager::Load(std::wstring wFileDirection, std::wstring wFileName) {
    // Conversion de wstring en string
    std::string fileDirection;
    for (wchar_t wc : wFileDirection) {
        fileDirection.push_back(static_cast<char>(wc));
    }

    // Conversion de wstring en string
    std::string FileName;
    for (wchar_t wc : wFileName) {
        FileName.push_back(static_cast<char>(wc));
    }

    addNewObject(FileName, meshVector);
}