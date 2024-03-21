#pragma once

#include "GameObject.h"


class Component {
public:
	virtual void JaiBesoinDunVirtual() {};


	explicit Component(GameObject* go) {
		linkedGameObject = go;
	};

protected:
	GameObject* linkedGameObject;
};