#include "PrefabManager.h"
#include <json.h>


PrefabManager::PrefabManager(Manager* manager) {
	manager->AddManager<PrefabManager>(this);

	//Rechercher les prefab
	SearchFile<PrefabManager>("Saves/Prefab", ".prefab", this);
}


void PrefabManager::Load(std::wstring wFileDirection) {
	
}

void PrefabManager::Save() { 

}
