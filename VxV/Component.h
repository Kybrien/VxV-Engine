#pragma once
#include "string"

class GameObject;


class Component {
public:
	virtual void JaiBesoinDunVirtual() {};


	explicit Component(GameObject* go) {
		linkedGameObject = go;
	};

	virtual std::string Save() = 0;
	virtual void Load() = 0;

	std::string name;

protected:
	GameObject* linkedGameObject;
};