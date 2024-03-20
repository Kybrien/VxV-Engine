#pragma once

#include <iostream>
#include <list>
#include <string>


#include "Component.h"


class GameObject 
{
public:
	std::string name; // Nom du GO

	virtual void Init() {}; // Est appelé au début 
	virtual void Start() {}; // Est appelé à la première frame
	virtual void Update() {}; // Est appelé à chaque frame

	int GetId() { return id; } // Renvoie l'Id du GameObject
	
	GameObject* GetChildByName(std::string name); // Rechercher un GameObject enfant du GameObject

	template<typename T>
	T* GetComponent(); // Rechercher un component


private:
	int id; // ID du GO


	std::list<Component*> components; // Liste des components
	std::list<GameObject*> childObjects; // Liste des enfants
};