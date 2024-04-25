#pragma once
#include "iostream"


class Script {
private:
	/**
	* @brief Represents the file direction associated with the script.
	* Stores the directory where the script file is located.
	*/
	std::string fileDirection;


public:
	/**
	* @brief Represents the name of the script.
	* Stores the name assigned to the script.
	*/
	std::string name;

	/**
	* @brief Constructs a Script object.
	* Represents constructing a Script object.
	*/
	Script();

	/**
	* @brief Retrieves the file direction associated with the script.
	* @return The file direction associated with the script.
	* Represents retrieving the file direction associated with the script.
	*/
	std::string getFile() {
		return fileDirection;
	}

	/**
	* @brief Sets the file direction associated with the script.
	* @param fileDirection_ The file direction to set.
	* Represents setting the file direction associated with the script.
	*/
	void SetFile(std::string fileDirection_) {
		fileDirection = fileDirection_;
	}

	/**
	* @brief Loads the script from the specified file.
	* @param _fileDirection The file direction where the script is located.
	* @param _name The name of the script.
	* Represents loading the script from the specified file.
	*/
	void Load(std::string _fileDirection, std::string _name);

	/**
	* @brief Called when the script starts.
	* Represents the start function of the script.
	*/
	void Start() {}

	/**
	* @brief Called every frame.
	* Represents the update function of the script.
	*/
	void Update() {}

};