#include "Script.h"
#include "Manager.h"
#include <iostream>

Script::Script(GameObject* gameObject, std::string _name) : Component(gameObject) {
	Manager* manager = Manager::GetInstance();
	scriptManager = manager->GetManager<ScriptManager>;
		type = Component::Type::Script;
		name = _name;
}
void Script::Start()
{
	std::cout << "Default Start() function called." << std::endl;
}

void Script::Update()
{
	std::cout << "Default Update() function called." << std::endl;
}
