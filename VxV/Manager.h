#pragma once
#include <vector>
#include <fstream>


class Manager {
protected:
	/**
	* @brief Pointer to the singleton instance of the Manager class.
	* Represents the singleton instance of the Manager class.
	*/
	static Manager* instance;


public:
	/**
	* @brief Retrieves the singleton instance of the Manager class.
	* @return Pointer to the singleton instance of the Manager class.
	* Represents retrieving the singleton instance of the Manager class.
	*/
	static Manager* GetInstance()
	{
		if (!instance)
			instance = new Manager;
		return instance;
	}

	/**
	* @brief Retrieves the manager of type T.
	* @tparam T The type of manager to retrieve.
	* @return Pointer to the manager of type T if found, otherwise nullptr.
	* Represents retrieving the manager of type T.
	*/
	template<typename T>
	T* GetManager();

	/**
	* @brief Initializes the Manager.
	* Represents initializing the Manager.
	*/
	void Init();

	/**
	* @brief Stops the Manager.
	* Represents stopping the Manager and deleting all pointers.
	*/
	void Stop(); // Supprimer tous les pointeurs

	/**
	* @brief Adds a manager to the Manager.
	* @tparam T The type of manager to add.
	* @param manager Pointer to the manager to add.
	* Represents adding a manager to the Manager.
	*/
	template<typename T>
	void AddManager(T* manager);



private:
	/**
	* @brief Vector containing pointers to Manager objects.
	* Represents a list of Manager objects.
	*/
	std::vector<Manager*> listManager;


protected:

	/**
	* @brief Searches for files with a specific extension in a given directory and processes them using the specified manager.
	* @tparam T The type of manager to use for processing the files.
	* @param fileDirection The directory to search for files.
	* @param extension The file extension to search for.
	* @param manager Pointer to the manager object used for processing the files.
	* Represents searching for files with a specific extension in a given directory and processing them using the specified manager.
	*/
	template<typename T>
	void SearchFile(std::string fileDirection, std::string extension, T* manager);

	/**
	* @brief Saves data associated with the object.
	* @details Override this function to define custom saving behavior for derived classes.
	* Represents saving data associated with the object.
	*/
	virtual void Save() {};

	/**
	* @brief Loads data associated with the object from the specified file.
	* @param wFileDirection The directory where the file is located.
	* @param wFileName The name of the file to load.
	* Represents loading data associated with the object from the specified file.
	*/
	virtual void Load(std::wstring wFileDirection, std::wstring wFileName) {};


};

#include "Manager.inl"