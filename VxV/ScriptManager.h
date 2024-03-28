#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Manager.h"
#include "Script.h"

class ScriptManager : public Manager {
private:
	std::vector<Script*> scripts;

public:

	ScriptManager(Manager* manager);

	void AddScript(Script* script) {
		scripts.push_back(script);
	}

	std::vector<Script*> GetScripts() {
		return scripts;
	}

	void Save() override;
	void Load(std::wstring wFileDirection) override;
};
