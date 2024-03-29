#include "ScriptManager.h"

ScriptManager::ScriptManager(Manager* manager) {

	manager->AddManager<ScriptManager>(this);
}

void ScriptManager::Save()
{
}

void ScriptManager::Load(std::wstring wFileDirection)
{
}
