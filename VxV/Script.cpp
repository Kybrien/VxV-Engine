#include "Script.h"
#include "ScriptManager.h"


Script::Script() {
	ScriptManager* sm = Manager::GetInstance()->GetManager<ScriptManager>();
	sm->AddScript(this);
}

void Script::Load(std::string _fileDirection, std::string _name) {
	fileDirection = _fileDirection;
	name = _name;
}