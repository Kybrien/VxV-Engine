#include "ScriptingComponent.h"
#include "ScriptManager.h"
#include "Debug.h"
#include <iostream>

ScriptingComponent::ScriptingComponent(GameObject* gameObject) : Component(gameObject) {
	scriptManager = Manager::GetInstance()->GetManager<ScriptManager>();


	linkedGameObject = gameObject;
	type = ScriptComponent;

	script = nullptr;
}


void ScriptingComponent::Load(Json::Value& compJson, GameObject* parentGO) {
	std::string temp = compJson["Script"].asString();
	name = compJson["Name"].asString();
	linkedGameObject = parentGO;

	for (Script* script : scriptManager->GetAllScripts()) {
		if (script->name == temp) {
			this->script = script;
			break;
		}
	}
}

void ScriptingComponent::Save(Json::Value& compJson) {
	compJson["Type"] = type;
	compJson["Name"] = name;
	compJson["Script"] = script->name;
}