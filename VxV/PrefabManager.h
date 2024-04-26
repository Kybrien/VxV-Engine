#pragma once
#include "Manager.h"
#include "Prefab.h"

class PrefabManager : public Manager {
private:
	/**
	* @brief Vector containing pointers to Prefab objects.
	*/
	std::vector<Prefab*> prefabs;

	/**
	* @brief Static string representing the file direction.
	* Used to store the directory where prefab files are located.
	*/
	static std::string fileDirection;

	/**
	* @brief Static string representing the file extension.
	* Used to specify the file extension for prefab files.
	*/
	static std::string extention;



public:
	/**
	* @brief Constructs a PrefabManager object.
	* @param manager Pointer to the Manager object associated with the PrefabManager.
	*/
	PrefabManager(Manager* manager);

	/**
	* @brief Retrieves the list of prefabs.
	* @return A vector containing pointers to Prefab objects.
	*/
	std::vector<Prefab*> GetPrefabs() {
		return prefabs;
	}

	/**
	* @brief Adds a prefab to the list of prefabs.
	* @param prefab Pointer to the Prefab object to be added.
	* @return void
	*/
	void AddPrefab(Prefab* prefab) {
		prefabs.push_back(prefab);
	}


	void Save() override;
	void Load(std::wstring wFileDirection, std::wstring wFileName) override;

};