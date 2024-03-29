#include "ScriptManager.h"
#include "GameObject.h"

std::string ScriptManager::fileDirection = "Scripts";

ScriptManager::ScriptManager(Manager* manager) {

	manager->AddManager<ScriptManager>(this);
}
void ScriptManager::AddScript(Script* script) {
	if (script) {
		scripts.push_back(script);
		script->GenerateScript(script->GetLinkedGameObject()->name + "Script");
	}
}
void ScriptManager::Save()
{

}

void ScriptManager::Load(std::wstring wFileDirection)
{

}
