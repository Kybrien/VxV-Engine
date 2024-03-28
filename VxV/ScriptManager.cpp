#include "ScriptManager.h"

ScriptManager::ScriptManager(Manager* manager) {

	manager->AddManager<ScriptManager>(this);
}
