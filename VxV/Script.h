#pragma once
#include "Component.h"
#include <glm/glm.hpp>

#include "json.h"


class ScriptManager;

using namespace glm;
class Script : public Component {

private:
	ScriptManager* scriptManager;


public:
	std::string name;
	Script(GameObject* gameObject, bool prefabLoading = false , std::string _name = "Script", bool alrExisting = false);


	void Load(Json::Value& compJson, GameObject* parentGO) override;

	void Save(Json::Value& compJson) override {
		compJson["Type"] = type;
		compJson["Script"] = name;
	}

	virtual void Start(); 
	virtual void Update();

	void Init();

};
