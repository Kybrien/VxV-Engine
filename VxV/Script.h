#pragma once
#include "Component.h"
#include <glm/glm.hpp>

#include "json.h"


using namespace glm;
class Script : public Component {
public:
	std::string name;
	Script(GameObject* gameObject, bool PrefabLoading = false , std::string _name = "scriptSample");

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
};
