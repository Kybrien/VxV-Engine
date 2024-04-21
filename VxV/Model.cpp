
#include "Model.h"
#include "GameObject.h"

#include "Debug.h"

#include "ModelManager.h"

Model::Model(GameObject* go) : Component(go) { 
	ModelManager* modelManager = Manager::GetInstance()->GetManager<ModelManager>();
	linkedGameObject = go;
	type = Component::Mesh;

	model = modelManager->GetModel("cube.obj");
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
	ModelManager* modelManager = Manager::GetInstance()->GetManager<ModelManager>();

	//Check s'il y a l'extention .obj
	std::string extentionCheck;
	if (modelName.size() >= 4) {
		extentionCheck = modelName.substr(modelName.size() - 4);
	}

	if(extentionCheck != ".obj")
		modelName += ".obj";

	modelManager->SetModel(modelName, linkedGameObject);
}