#pragma once
#include "Manager.h"
#include "Prefab.h"

class PrefabManager : public Manager {
private:
	std::vector<Prefab*> prefabs;

	static std::string fileDirection;
	static std::string extention;


public:
	PrefabManager(Manager* manager);

	std::vector<Prefab*> GetPrefabs() {
		return prefabs;
	}

	void AddPrefab(Prefab* prefab) {
		prefabs.push_back(prefab);
	}

	void Save() override;
	void Load(std::wstring wFileDirection, std::wstring wFileName) override;

};