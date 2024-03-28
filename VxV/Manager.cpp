#include "Manager.h"
#include "SceneManager.h"
#include "PrefabManager.h"


Manager* Manager::instance = nullptr;

void Manager::Init() {
	new PrefabManager(this);
	new SceneManager(this);
}





