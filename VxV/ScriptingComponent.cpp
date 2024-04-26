#include "ScriptingComponent.h"
#include "ScriptManager.h"
#include "Debug.h"
#include "GameObject.h"
#include <iostream>


ScriptingComponent::ScriptingComponent() : Component() {
	script = nullptr;
	name = "ScriptingComponent";
}

ScriptingComponent::ScriptingComponent(GameObject* gameObject) : Component(gameObject) {
	scriptManager = Manager::GetInstance()->GetManager<ScriptManager>();


	linkedGameObject = gameObject;
	type = ScriptComponent;

	script = nullptr;
}


void ScriptingComponent::Load(Json::Value& compJson, GameObject* parentGO) {
	linkedGameObject = parentGO;

	if (compJson.isMember("Script")) {

	std::string temp = compJson["Script"].asString();

	for (Script* script : scriptManager->GetAllScripts()) {
		if (script->name == temp + ".cpp") {
			this->script = script;
			break;
		}
	}
	}
}

void ScriptingComponent::Save(Json::Value& compJson) {
	compJson["Type"] = type;

	if (script != nullptr) {
		compJson["Script"] = script->name;
	}
}

void ScriptingComponent::Copy(GameObject* goToFill) {

	goToFill->AddComponent<ScriptingComponent>();

	ScriptingComponent* newScriptComp = goToFill->GetComponent<ScriptingComponent>();

	newScriptComp->script = script;

}
