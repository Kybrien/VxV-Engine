#pragma once

#include <iostream>
#include <list>
#include <string>

#include "Component.h"


class GameObject 
{
public:
	GameObject() {
		components.push_back(new Transform(this));
		// Reucp scene avec manager
		// origin = scene.origin 
	}

	glm::vec3 origin;


	std::string name; // Nom du GO
	virtual void Init() {}; // Est appelé au début 
	virtual void Start() {}; // Est appelé à la première frame
	virtual void Update() {}; // Est appelé à chaque frame

	const int GetId() { return id; } // Renvoie l'Id du GameObject
	
	GameObject* GetChildByName(std::string name); // Rechercher un GameObject enfant du GameObject


	void AddChild(GameObject* go);

	template<typename T>
	T* GetComponent(); // Rechercher un component

	template<typename T>
	void AddComponent();


private:
	int id; // ID du GO


	std::list<Component*> components; // Liste des components
	std::list<GameObject*> childObjects; // Liste des enfants
};