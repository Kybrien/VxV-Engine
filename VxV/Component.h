#pragma once
#include "json.h"

class GameObject;
class Transform;

class Component
{
public:
	Component(GameObject* go)
	{
		linkedGameObject = go;
		type = Unknown;
	};

	virtual void Save(Json::Value& compJson) = 0;
	virtual void Load(Json::Value& compJson, GameObject* ParentGO) = 0;

	virtual void Copy(GameObject* goToFill) = 0;

	// Enumeration de tous les types de Components
	enum Type
	{
		Unknown,
		Transform,
		Mesh,
		ScriptComponent,
	};

	Type type;

	GameObject* GetLinkedGameObject() const
	{
		return linkedGameObject;
	}

protected:
	GameObject* linkedGameObject;
};
