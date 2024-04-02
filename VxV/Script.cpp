#include "Script.h"
#include "ScriptManager.h"
#include "Debug.h"
#include <iostream>

Script::Script(GameObject* gameObject, bool Prefabloading, std::string _name, bool alrExisting) : Component(gameObject) {

    scriptManager = Manager::GetInstance()->GetManager<ScriptManager>();
    linkedGameObject = gameObject;

	type = Component::Type::Script;



    bool nameFound = false;
    int i = 1;
    std::string nameChecking = _name;

    while (!nameFound) {

        bool nameTaken = false;
        for (Script* scr : scriptManager->GetScripts()) {
            if (scr->name == nameChecking) {
                nameTaken = true;
                break;
            }
        }

        if (nameTaken) {
            nameChecking = _name + "(" + std::to_string(i) + ")";
        }
        else {
            nameFound = true;
        }

        i++;

    }

    name = nameChecking;


    if (!Prefabloading) {

        scriptManager->AddScript(this, alrExisting);
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


void Script::Load(Json::Value& compJson, GameObject* parentGO) {

    bool scriptFound = false;

    while (!scriptFound)
    {
        for (Script* script : scriptManager->GetScripts()) {
            if (compJson["Script"].asString() == script->name) {
                linkedGameObject = parentGO;
                scriptFound = true;
                break;
            }
        }

        if (!scriptFound) {
            Debug::Log("Impossible de trouver le script " + compJson["Script"].asString() + "\n");
            scriptManager->AddScript(new Script(parentGO, false, compJson["Script"].asString(), false));
        }
    }


}