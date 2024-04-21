#pragma once
#include "Component.h"
#include "Script.h"


class ScriptManager;

class ScriptingComponent : public Component {

private:
	ScriptManager* scriptManager;


public:
	std::string name = "ScriptComponent";
	Script* script;

	ScriptingComponent(GameObject* gameObject);

	void AddScript(Script* script) {
		this->script = script;
	}
	
	Script* GetScript() {
		return script;
	}


	void Load(Json::Value& compJson, GameObject* parentGO) override; 

	void Save(Json::Value& compJson) override; 
	 
	void Copy(GameObject* goToFill) override;
};
