#pragma once
#include "Manager.h"
#include "Prefab.h"

class PrefabManager : public Manager {
private:
	std::vector<Prefab> prefabs;

	std::string fileDirection = "Saves/Prefab";
	std::string extention = ".prefab";


public:
	PrefabManager(Manager* manager);

	void Save() override;
	void Load(std::wstring wFileDirection) override; 
};