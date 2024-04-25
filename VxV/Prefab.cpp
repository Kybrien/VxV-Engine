#include "Prefab.h"
#include"PrefabManager.h"
#include <GLM/glm.hpp>

#include "Debug.h"


Prefab::Prefab(GameObject* go, std::string name) {

	this->name = name;
	gameObject = new GameObject("", true);
	Copy(gameObject, go);
	gameObject->isChild = false;
	Manager::GetInstance()->GetManager<PrefabManager>()->AddPrefab(this);

	Manager::GetInstance()->GetManager<PrefabManager>()->Save();
}



// Copier le go du prefab sur un go
GameObject* Prefab::Copy(GameObject* goToFill, GameObject* goToCopy) {
	goToFill->currentScene = goToCopy->currentScene;

	goToFill->origin = glm::vec3(goToCopy->origin.x, goToCopy->origin.y, goToCopy->origin.z);

	goToFill->name = goToCopy->name;

	for (GameObject* child : goToCopy->GetChilds()) {
		GameObject* newGO = new GameObject("", false, nullptr, true);
		goToFill->AddChild(newGO);
		Copy(newGO, child);
	}

	for (auto comp : goToCopy->GetComponents()) {
		comp->Copy(goToFill);
	}

	return goToFill;
}