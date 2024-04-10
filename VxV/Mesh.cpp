
#include "Mesh.h"
#include "GameObject.h"

#include "MeshManager.h"

Mesh::Mesh(GameObject* go) : Component(go) {
	MeshManager* meshManager = Manager::GetInstance()->GetManager<MeshManager>();
	linkedGameObject = go;
	type = Component::Mesh;

	mesh = meshManager->GetMesh("cube.obj");
}

void Mesh::Load(Json::Value& compJson, GameObject* parentGO) {
	linkedGameObject = parentGO;
}

void Mesh::Save(Json::Value& compJson) {
	compJson["Type"] = type;
}

void Mesh::Copy(GameObject* goToFill) {

	goToFill->AddComponent<Mesh>();

	Mesh* newMeshComp = goToFill->GetComponent<Mesh>();

	newMeshComp->mesh = mesh;
}