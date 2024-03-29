#include "Manager.h"
#include "SceneManager.h"
#include "PrefabManager.h"
#include "ScriptManager.h"


Manager* Manager::instance = nullptr;

void Manager::Init() {
	new ScriptManager(this);
	new PrefabManager(this);
	new SceneManager(this);
}





