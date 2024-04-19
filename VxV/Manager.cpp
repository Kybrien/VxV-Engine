#include "Manager.h"
#include "SceneManager.h"
#include "PrefabManager.h"
#include "ScriptManager.h"
#include "ModelManager.h"


Manager* Manager::instance = nullptr;

void Manager::Init() {
	new SceneManager(this);
	new ModelManager(this);
	new ScriptManager(this);
	new PrefabManager(this);
}