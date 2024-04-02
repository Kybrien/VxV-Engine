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

	void AddScript(Script* script, bool loading = false);


	void GenerateScript(const std::string& className, const std::string& templateFileName = "ScriptUser.cpp");

	std::vector<Script*> GetScripts() {
		return scripts;
	}

	void Save() override;
	void Load(std::wstring wFileDirection, std::wstring wFileName) override;

	std::string fileDirection = "Scripts";
	std::string extention = ".cpp";
};
