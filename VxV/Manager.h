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

	template<typename T>
	T* GetManager() {
		for (Manager* manager : listManager) {
			T* typedManager = dynamic_cast<T*>(manager);
			if (typedManager != nullptr) {
				return typedManager; // Retourner le composant du type T s'il est trouvé
			}
		}

		return nullptr; // Aucun composant de type T trouvé
	}

	void Init();

	void Stop(); // Supprimer tous les pointeurs

	template<typename T>
	void AddManager(T* manager) {
		static_assert(std::is_base_of<Manager, T>::value, "T doit être un descendant de Manager");

		// Ajoute un nouveau composant de type T
		listManager.push_back(manager);
	}


private:

	std::vector<Manager*> listManager;

protected:

	template<typename T>
	void SearchFile(std::string fileDirection, std::string extension, T* manager);

	virtual void Save() {}; 
	virtual void Load(std::wstring wFileDirection, std::wstring wFileName) {};



};

#include "Manager.inl"