#pragma once

#include <iostream>
#include <list>
#include <string>


#include "Component.h"


class GameObject 
{
public:
	std::string name;

	virtual void Init() {};
	virtual void Start() {};
	virtual void Update() {};

	int GetId() { return id; }

	GameObject* GetChildByName(std::string name);

	template<typename T>
	T* GetComponent();


private:
	int id;


	std::list<Component*> components;
	std::list<GameObject*> childObjects;
};