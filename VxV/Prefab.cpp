#include "Prefab.h"
#include"PrefabManager.h"

Prefab::Prefab(GameObject &go, std::string name) {

	gameObject = new GameObject("", true);

	*gameObject = go;
	Manager::GetInstance()->GetManager<PrefabManager>()->AddPrefab(this);
}