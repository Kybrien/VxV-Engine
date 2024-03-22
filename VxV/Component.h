#pragma once
#include "Transform.h"

class GameObject;


class Component {
public:
	virtual void JaiBesoinDunVirtual() {};


	explicit Component(GameObject* go) {
		linkedGameObject = go;
	};

	virtual void Save() = 0;
	virtual void Load() = 0;

protected:
	GameObject* linkedGameObject;
};