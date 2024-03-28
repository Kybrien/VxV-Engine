#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class ScriptManager;

using namespace glm;
class Script : public Component {
public:
	std::string name;
	Script(GameObject* gameObject, std::string _name = "scriptSample") : Component(gameObject) {};

	void Load(Json::Value& compJson) override {
		name = compJson["Script"].asString();
	}

	void Save(Json::Value& compJson) override {
		compJson["Type"] = type;
		compJson["Script"] = name;
	}

	virtual void Start(); 
	virtual void Update();

	void Init();
private:
	ScriptManager* scriptManager;
};
