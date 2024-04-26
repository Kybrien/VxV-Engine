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
	/**
* @brief Pointer to the current Scene.
* Represents the currently active scene.
*/
	Scene* currentScene;

	/**
	* @brief Vector containing pointers to Component objects.
	* Represents the components attached to the GameObject.
	*/
	std::vector<Component*> components;

	/**
	* @brief Constructs a GameObject object with optional parameters.
	* @param name_ The name of the GameObject. Default is "GO".
	* @param PrefabLoading Specifies if the GameObject is being loaded from a prefab. Default is false.
	* @param prefab Pointer to the Prefab if the GameObject is being instantiated from a prefab. Default is nullptr.
	* @param copying Specifies if the GameObject is being copied. Default is false.
	*/
	GameObject(std::string name_ = "GO", bool PrefabLoading = false, Prefab* prefab = nullptr, bool copying = false);

	/**
	* @brief Vector representing the origin point of the GameObject.
	*/
	glm::vec3 origin;

	/**
	* @brief Indicates if the GameObject is a child of another GameObject.
	* Default is false.
	*/
	bool isChild = false;


	/**
	* @brief Name of the GameObject.
	* Represents the name assigned to the GameObject.
	*/
	std::string name; // Nom du GO

	/**
	* @brief Initializes the GameObject.
	* This function is called at the beginning.
	* Override this function to perform initialization tasks.
	*/
	virtual void Init() {}; // Est appelé au début

	/**
	* @brief Starts the GameObject.
	* This function is called at the first frame.
	* Override this function to perform start-up tasks.
	*/
	virtual void Start() {}; // Est appelé à la première frame

	/**
	* @brief Updates the GameObject.
	* This function is called at each frame.
	* Override this function to perform update tasks.
	*/
	virtual void Update() {}; // Est appelé à chaque frame

	/**
	* @brief Gets the ID of the GameObject.
	* @return The ID of the GameObject.
	* Represents the unique identifier assigned to the GameObject.
	*/
	const int GetId() { return id; } // Renvoie l'Id du GameObject

	/**
	* @brief Sets the ID of the GameObject.
	* @param id_ The ID to set.
	* Represents the unique identifier to assign to the GameObject.
	*/
	void SetId(int id_) { id = id_; }


	/**
	* @brief Retrieves a child GameObject by its name.
	* @param name The name of the child GameObject to search for.
	* @return Pointer to the child GameObject if found, otherwise nullptr.
	* Represents searching for a child GameObject of the current GameObject by its name.
	*/
	GameObject* GetChildByName(std::string name); // Rechercher un GameObject enfant du GameObject

	/**
	* @brief Retrieves all components attached to the GameObject.
	* @return A vector containing pointers to all Component objects attached to the GameObject.
	* Represents retrieving all components attached to the current GameObject.
	*/
	std::vector<Component*> GetComponents();

	/**
	* @brief Retrieves all child GameObjects of the GameObject.
	* @return A vector containing pointers to all child GameObjects.
	* Represents retrieving all child GameObjects of the current GameObject.
	*/
	std::vector<GameObject*> GetChilds();

	/**
	* @brief Retrieves the parent GameObject.
	* @return Pointer to the parent GameObject.
	* Represents retrieving the parent GameObject of the current GameObject.
	*/
	GameObject* GetParent()
	{
		return parent;
	}

	/**
	* @brief Loads a GameObject from JSON data.
	* @param root The root JSON value containing the GameObject data.
	* @param goParent Pointer to the parent GameObject if applicable. Default is nullptr.
	* @param PrefabLoading Specifies if the GameObject is being loaded from a prefab. Default is false.
	* Represents loading a GameObject from JSON data.
	*/
	static void Load(Json::Value root, GameObject* goParent = nullptr, bool PrefabLoading = false);

	/**
	* @brief Saves the GameObject to JSON data.
	* @param root Reference to the root JSON value to which the GameObject data will be saved.
	* Represents saving the GameObject to JSON data.
	*/
	void Save(Json::Value& root);


	/**
	* @brief Adds a child GameObject to the current GameObject.
	* @param go Pointer to the child GameObject to add.
	* Represents adding a child GameObject to the current GameObject.
	*/
	void AddChild(GameObject* go);

	/**
	* @brief Sets the parent GameObject for the current GameObject.
	* @param go Pointer to the parent GameObject to set.
	* Represents setting the parent GameObject for the current GameObject.
	*/
	void AddParent(GameObject* go)
	{
		parent = go;
	}

	/**
	* @brief Retrieves the name of the prefab associated with the GameObject.
	* @return The name of the prefab associated with the GameObject.
	* Represents retrieving the name of the prefab associated with the GameObject.
	*/
	std::string getPrefabName()
	{
		return prefabName;
	}

	/**
	* @brief Sets the prefab associated with the GameObject.
	* @param prefab_ Pointer to the Prefab object to set.
	* Represents setting the prefab associated with the GameObject.
	*/
	void SetPrefab(Prefab* prefab_)
	{
		prefab = prefab_;
	}

	/**
	* @brief Retrieves the prefab associated with the GameObject.
	* @return Pointer to the Prefab object associated with the GameObject.
	* Represents retrieving the prefab associated with the GameObject.
	*/
	Prefab* GetPrefab()
	{
		return prefab;
	}


	/**
	* @brief Removes a child GameObject from the current GameObject.
	* @param goChild Pointer to the child GameObject to remove.
	* Represents removing a child GameObject from the current GameObject.
	*/
	void RemoveChild(GameObject* goChild);

	/**
	* @brief Retrieves a component of type T attached to the GameObject.
	* @tparam T The type of component to search for.
	* @return Pointer to the component of type T if found, otherwise nullptr.
	* Represents searching for a component of type T attached to the GameObject.
	*/
	template<typename T>
	T* GetComponent(); // Rechercher un component

	/**
	* @brief Adds a component of type T to the GameObject.
	* @tparam T The type of component to add.
	* Represents adding a component of type T to the GameObject.
	*/
	template<typename T>
	void AddComponent();

	/**
	* @brief Loads a component from JSON data and attaches it to the GameObject.
	* @param compJson The JSON value containing the component data.
	* @param parentGo Pointer to the parent GameObject if applicable.
	* Represents loading a component from JSON data and attaching it to the GameObject.
	*/
	void LoadComponent(Json::Value compJson, GameObject* parentGo);

	/**
	* @brief Deletes a GameObject and frees its memory.
	* @param go Pointer to the GameObject to delete.
	* Represents deleting a GameObject and freeing its memory.
	*/
	static void Delete(GameObject* go);


private:
	/**
	* @brief ID of the GameObject.
	* Represents the unique identifier assigned to the GameObject.
	*/
	int id; // ID du GO

	/**
	* @brief List of child GameObjects.
	* Represents the GameObjects that are children of the current GameObject.
	*/
	std::vector<GameObject*> childObjects; // Liste des enfants

	/**
	* @brief Pointer to the parent GameObject.
	* Represents the parent GameObject of the current GameObject.
	*/
	GameObject* parent;

	/**
	* @brief Name of the prefab associated with the GameObject.
	* Represents the name of the prefab associated with the GameObject.
	*/
	std::string prefabName;

	/**
	* @brief Pointer to the Prefab associated with the GameObject.
	* Represents the Prefab associated with the GameObject.
	*/
	Prefab* prefab;

};


#include "GameObject.inl"