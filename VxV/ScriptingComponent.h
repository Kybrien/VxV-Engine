#pragma once
#include "Component.h"
#include "Script.h"


class ScriptManager;

class ScriptingComponent : public Component {

private:
	/**
	* @brief An instance of the script Manager
	*/
	ScriptManager* scriptManager;

	/**
	* @brief The script assigned to the component
	*/
	Script* script;


public:
	/**
	* @brief Script's name
	*/
	std::string name = "ScriptComponent";

	/**
	* @brief Instanciate a component Script.
	* @param A reference to the linked GameObject
	*/
	ScriptingComponent(GameObject* gameObject);
	ScriptingComponent();

	/**
	* @brief Assign a script to the component.
	* @param A reference to the the script to assign
	* @return void
	*/
	void AddScript(Script* script) {
		this->script = script;
	}
	
	/**
	* @brief Get the script assigned to the component.
	* @return Script*
	*/
	Script* GetScript() {
		return script;
	}


	void Save(Json::Value& compJson) override;

	void Load(Json::Value& compJson, GameObject* ParentGO) override;

	void Copy(GameObject* goToFill) override;
};
