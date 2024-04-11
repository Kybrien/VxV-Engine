
#include "Mesh.h"
#include "GameObject.h"

#include "Debug.h"

#include "MeshManager.h"

Mesh::Mesh(GameObject* go) : Component(go) {
	MeshManager* meshManager = Manager::GetInstance()->GetManager<MeshManager>();
	linkedGameObject = go;
	type = Component::Mesh;

	mesh = meshManager->GetMesh("cube.obj");
	mesh->numberUsed++;
}

void Mesh::Load(Json::Value& compJson, GameObject* parentGO) {
	linkedGameObject = parentGO;

	SetMesh(compJson["Mesh"].asString());
}

void Mesh::Save(Json::Value& compJson) {
	compJson["Type"] = type;
	compJson["Mesh"] = mesh->fileName;
}

void Mesh::Copy(GameObject* goToFill) {

	goToFill->AddComponent<Mesh>();

	Mesh* newMeshComp = goToFill->GetComponent<Mesh>();

	newMeshComp->mesh = mesh;
}


void Mesh::SetMesh(std::string meshName) {
	MeshManager* meshManager = Manager::GetInstance()->GetManager<MeshManager>();

	//Check s'il y a l'extention .obj
	std::string extentionCheck;
	if (meshName.size() >= 4) {
		extentionCheck = meshName.substr(meshName.size() - 4);
	}

	if(extentionCheck != ".obj")
		meshName += ".obj";

	meshManager->SetMesh(meshName, linkedGameObject);
}