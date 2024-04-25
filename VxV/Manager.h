#pragma once
#include <vector>
#include <fstream>


class Manager {
protected:

	static Manager* instance;

public:
	static Manager* GetInstance()
	{
		if (!instance)
			instance = new Manager;
		return instance;
	}

	/**
	* @brief Return the manager of the type T
	* @return T*
	*/
	template<typename T>
	T* GetManager();

	void Init();

	void Stop(); // Supprimer tous les pointeurs

	template<typename T>
	void AddManager(T* manager);


private:

	std::vector<Manager*> listManager;

protected:

	template<typename T>
	void SearchFile(std::string fileDirection, std::string extension, T* manager);

	virtual void Save() {}; 
	virtual void Load(std::wstring wFileDirection, std::wstring wFileName) {};



};

#include "Manager.inl"