#pragma once
#include "json.h"

class GameObject;
class Transform;

class Component
{
public:
	/**
	* @brief Constructs a Component object and links it to a GameObject.
	* @param go Pointer to the GameObject to which the component is linked.
	*/
	Component(GameObject* go)
	{
		linkedGameObject = go;
		type = Unknown;
	};
	Component() {};

	std::string name;

	/**
	* @brief Save the component in a Json Object
	* @param A Json Value used by the lib jsoncpp to save the data
	* @return void
	*/
	virtual void Save(Json::Value& compJson) = 0;

	/**
	* @brief Load the component from a Json Object
	* @param A Json Value used by the lib jsoncpp to save the data
	* @param A reference to the parent GameObject
	* @return void
	*/
	virtual void Load(Json::Value& compJson, GameObject* ParentGO) = 0;

	/**
	* @brief Copy the value of a component to another gameObject
	* @param A reference to the GameObject to fill
	* @return void
	*/
	virtual void Copy(GameObject* goToFill) = 0;


	/**
	* @brief Enumeration representing different types of components.
	*/
	enum Type
	{
		Unknown,            /**< Unknown type. */
		Transform,          /**< Transform component. */
		Model,               /**< Mesh component. */
		ScriptComponent, 
		Light,/**< Script component. */
	};

	/**
	* @brief Type of the component.
	*/
	Type type;

	/**
	* @brief Retrieves the linked GameObject of the component.
	* @return Pointer to the linked GameObject.
	*/
	GameObject* GetLinkedGameObject() const
	{
		return linkedGameObject;
	}


protected:
	/**
	* @brief Pointer to the GameObject linked to the component.
	*/
	GameObject* linkedGameObject;

};
