#include "Script.h"
#include "ScriptManager.h"


Script::Script() {
	ScriptManager scriptManager = Manager::GetInstance()->GetManager<ScriptManager>();

	scriptManager.GetAllScripts().push_back(this);
}

void Script::Load(std::string _fileDirection, std::string _name) {
	fileDirection = _fileDirection;
	name = _name;
}