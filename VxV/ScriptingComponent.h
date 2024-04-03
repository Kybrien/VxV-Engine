#pragma once
#include "Component.h"
#include <glm/glm.hpp>

#include "json.h"
#include "Script.h"


class ScriptManager;

using namespace glm;
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


	void Load(Json::Value& compJson, GameObject* parentGO) override;

	void Save(Json::Value& compJson) override;
};
