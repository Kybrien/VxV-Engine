#include "Script.h"
#include "ScriptManager.h"


Script::Script() {
	Manager::GetInstance()->GetManager<ScriptManager>()->GetAllScripts().push_back(this);
}

void Script::Load(std::string _fileDirection, std::string _name) {
	fileDirection = _fileDirection;
	name = _name;
}