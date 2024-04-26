#pragma once
#include <string>
#include <vector>

#include "Manager.h"
#include "Script.h"

class ScriptManager : public Manager
{
private:
	/**
* @brief Vector containing pointers to Script objects.
*/
	std::vector<Script*> scripts;

	/**
	* @brief Generates a script file based on the provided class name and template file.
	* @param className The name of the class to generate the script for.
	* @param templateFileName The name of the template file to use for generating the script.
	*                         Default value is "ScriptUser.cpp" if not specified.
	* @return void
	*/
	static void GenerateScript(const std::string& className, const std::string& templateFileName = "ScriptUser.cpp");


public:
	/**
	* @brief Constructs a ScriptManager object.
	* @param manager Pointer to the Manager object associated with the ScriptManager.
	*/
	ScriptManager(Manager* manager);

	/**
	* @brief Retrieves all scripts managed by the ScriptManager.
	* @return A vector containing pointers to all Script objects.
	*/
	std::vector<Script*> GetAllScripts()
	{
		return scripts;
	}

	/**
	* @brief Adds a script to the ScriptManager.
	* @param script Pointer to the Script object to be added.
	* @return void
	*/
	void AddScript(Script* script)
	{
		scripts.push_back(script);
	}


	void Save() override;
	void Load(std::wstring wFileDirection, std::wstring wFileName) override;

	/**
	* @brief Static string representing the file direction.
	* Used to store the directory where files are located.
	*/
	static std::string fileDirection;

	/**
	* @brief Static string representing the file extension.
	* Used to specify the file extension for files.
	*/
	static std::string extention;

	/**
	* @brief Creates a new Script object with the given name.
	* @param name The name of the script.
	* @return A pointer to the newly created Script object.
	*/
	static Script* NewScript(std::string name);

};
