#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Manager.h"
#include "ScriptingComponent.h"
#include "Script.h"

class ScriptManager : public Manager {
private:
	std::vector<Script*> scripts;


	static void GenerateScript(const std::string& className, const std::string& templateFileName = "ScriptUser.cpp");

public:

	ScriptManager(Manager* manager);

	std::vector<Script*> GetAllScripts() {
		return scripts;
	}






	void Save() override;
	void Load(std::wstring wFileDirection, std::wstring wFileName) override;

	static std::string fileDirection;
	static std::string extention;


	static void NewScript(std::string name);
};
