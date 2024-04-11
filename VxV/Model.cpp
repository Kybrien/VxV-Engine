
#include "Model.h"
#include "GameObject.h"

#include "Debug.h"

#include "MeshManager.h"

Model::Model(GameObject* go) : Component(go) { 
	MeshManager* meshManager = Manager::GetInstance()->GetManager<MeshManager>();
	linkedGameObject = go;
	type = Component::Mesh;

	model = meshManager->GetMesh("cube.obj");
	model->numberUsed++;
}

void Model::Load(Json::Value& compJson, GameObject* parentGO) {
	linkedGameObject = parentGO;

	SetModel(compJson["Model"].asString());
}

void Model::Save(Json::Value& compJson) {
	compJson["Type"] = type;
	compJson["Model"] = model->fileName;
}

void Model::Copy(GameObject* goToFill) {

	goToFill->AddComponent<Model>();

	Model* newModelComp = goToFill->GetComponent<Model>();

	newModelComp->model = model;
}


void Model::SetModel(std::string modelName) {
	MeshManager* meshManager = Manager::GetInstance()->GetManager<MeshManager>();

	//Check s'il y a l'extention .obj
	std::string extentionCheck;
	if (modelName.size() >= 4) {
		extentionCheck = modelName.substr(modelName.size() - 4);
	}

	if(extentionCheck != ".obj")
		modelName += ".obj";

	meshManager->SetModel(modelName, linkedGameObject);
}