#include "Script.h"
#include "ScriptManager.h"
#include <iostream>

Script::Script(GameObject* gameObject, bool prefabLoading, std::string _name) : Component(gameObject) {
	type = Component::Type::Script;
	name = _name;

	if (!prefabLoading) {

		ScriptManager* scriptManager = Manager::GetInstance()->GetManager<ScriptManager>();
		scriptManager->AddScript(this);
	}
}

void Script::Start()
{
	std::cout << "Default Start() function called." << std::endl;
}

void Script::Update()
{
	std::cout << "Default Update() function called." << std::endl;
}
