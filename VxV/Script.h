#pragma once
#include "iostream"


class Script {
private:
	std::string fileDirection;

public:
	std::string name;

	Script();

	std::string getFile() {
		return fileDirection;
	}

	void SetFile(std::string fileDirection_) {
		fileDirection = fileDirection_;
	}

	void Load(std::string _fileDirection, std::string _name);

	void Start() {}
	void Update() {}
};