#pragma once
#include "string"
#include "json.h"


class GameObject;
class Transform;


class Component {
public:
	virtual void JaiBesoinDunVirtual() {};


	explicit Component(GameObject* go) {
		linkedGameObject = go;
	};

	virtual void Save(Json::Value &compJson) = 0;
	virtual void Load(Json::Value& compJson) = 0;

	

	// Enumeration de tous les types de Components
	enum Type {
		Transform,
		Mesh_renderer,
		Script
	};

	Type type;

protected:
	GameObject* linkedGameObject;
};