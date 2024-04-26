#include "Manager.h"
#include "SceneManager.h"
#include "PrefabManager.h"
#include "ScriptManager.h"
#include "ModelManager.h"


Manager* Manager::instance = nullptr;

void Manager::Init() {
	new ModelManager(this);
	new SceneManager(this);
	new PrefabManager(this);
	new ScriptManager(this);
}