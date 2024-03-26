#pragma once
#include "string"
#include "json.h"

class GameObject;


class Component {
public:
	virtual void JaiBesoinDunVirtual() {};


	explicit Component(GameObject* go) {
		linkedGameObject = go;
	};

	virtual void Save(Json::Value &compJson) = 0;
	virtual void Load() = 0;

	std::string name;

protected:
	GameObject* linkedGameObject;
};