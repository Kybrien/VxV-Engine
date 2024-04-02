#pragma once
#include "GameObject.h"

class Prefab {
private: 
	GameObject gameObject;

public:
	std::string name;

	Prefab(GameObject &go, std::string name = "PrefabSample");

	GameObject getGameObject() {
		return gameObject;
	}
};