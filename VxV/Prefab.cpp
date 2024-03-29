#include "Prefab.h"
#include"PrefabManager.h"

Prefab::Prefab(GameObject &go, std::string name) {

	gameObject = go;
	gameObject.isChild = false;
	Manager::GetInstance()->GetManager<PrefabManager>()->AddPrefab(this);
}