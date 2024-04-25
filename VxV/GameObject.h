#pragma once
#include <json.h>
#include <string>
#include <vector>
#include <GLM/glm.hpp>
#include "Component.h"

#include "Transform.h"

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

	/**
	* @brief First function called when the GameObject is created
	*/
	virtual void Init() {};
	/**
	* @brief Function called at the first frame
	*/
	virtual void Start() {}; 
	/**
	* @brief Function called at each frame
	*/
	virtual void Update() {};

	/**
	* @brief Return the Id of the GameObject
	* @return int : Id of the GameObject
	*/
	const int GetId() { return id; }
	void SetId(int id_) { id = id_; }

	/**
	* @brief Return the first child GameObject with the name
	* @param std::string name : Child with the name
	*/
	GameObject* GetChildByName(std::string name);

	/**
	* @brief Return a vector of all the GameObject's components
	* @return std::vector<Component*> : Vector of all the GameObject's components
	*/
	std::vector<Component*> GetComponents();

	/**
	* @brief Return a vector of all the GameObject's children
	* @return std::vector<GameObject*> : Vector of all the GameObject's children
	*/
	std::vector<GameObject*> GetChilds();


	/**
	* @brief Return the Parent of the GameObject
	* @return GameObject* : Parent of the GameObject
	*/
	GameObject* GetParent()
	{
		return parent;
	}

	void static Load(Json::Value root, GameObject* goParent = nullptr, bool PrefabLoading = false);
	void Save(Json::Value& root);

	/**
	* @brief Add a child to the GameObject
	*/
	void AddChild(GameObject* go);

	/**
	* @brief Set the parent of the GameObject
	*/
	void AddParent(GameObject* go)
	{
		parent = go;
	}

	/**
	* @brief Remove a child from the GameObject
	*/
	void RemoveChild(GameObject* goChild);

	// Mettre des enable if
	/**
	* @brief Return the given component of the GameObject
	* @return T* : Component of the GameObject
	*/
	template<typename T>
	T* GetComponent(); // Rechercher un component

	/**
	* @brief Add the given component to the GameObject
	*/
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
