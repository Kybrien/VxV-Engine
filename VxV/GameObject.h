#pragma once
#include <json.h>
#include <string>
#include <vector>
#include <GLM/glm.hpp>
#include "Component.h"

class Scene;
class Prefab;

class GameObject
{
public:
	Scene* currentScene;

	std::vector<Component*> components;

	GameObject(std::string name_ = "GO", bool PrefabLoading = false, Prefab* prefab = nullptr, bool copying = false);

	glm::vec3 origin;
	bool isChild = false;

	std::string name; // Nom du GO
	virtual void Init() {}; // Est appelé au début 
	virtual void Start() {}; // Est appelé à la première frame
	virtual void Update() {}; // Est appelé à chaque frame

	const int GetId() { return id; } // Renvoie l'Id du GameObject
	void SetId(int id_) { id = id_; }

	GameObject* GetChildByName(std::string name); // Rechercher un GameObject enfant du GameObject
	std::vector<Component*> GetComponents();

	std::vector<GameObject*> GetChilds();

	GameObject* GetParent()
	{
		return parent;
	}

	void static Load(Json::Value root, GameObject* goParent = nullptr, bool PrefabLoading = false);
	void Save(Json::Value& root);

	void AddChild(GameObject* go);

	void AddParent(GameObject* go)
	{
		parent = go;
	}

	void RemoveChild(GameObject* goChild);

	// Mettre des enable if
	template<typename T>
	T* GetComponent(); // Rechercher un component

	template<typename T>
	void AddComponent();

	void LoadComponent(Json::Value compJson, GameObject* parentGo);

	static void Delete(GameObject* go);

private:
	int id; // ID du GO
	std::vector<GameObject*> childObjects; // Liste des enfants
	GameObject* parent;
	Prefab* prefab;
};


#include "GameObject.inl"