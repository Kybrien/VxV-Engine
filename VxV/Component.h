#pragma once
#include "string"
#include "json.h"


class GameObject;
class Transform;


class Component {
public:
	virtual void JaiBesoinDunVirtual() {};


	Component(GameObject* go) {
		linkedGameObject = go;
		type = Unknown;
	};

	virtual void Save(Json::Value &compJson) = 0;
	virtual void Load(Json::Value& compJson) = 0;

	

	// Enumeration de tous les types de Components
	enum Type {
		Unknown,
		Transform,
		Mesh_renderer,
		Script,
	};

	Type type;

	GameObject* GetLinkedGameObject() const {
		return linkedGameObject;
	}

protected:
	GameObject* linkedGameObject;
};