#pragma once
#include <vector>
#include <fstream>


class SceneManager;

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

	template<typename T>
	void AddManager(T* manager) {
		static_assert(std::is_base_of<Manager, T>::value, "T doit être un descendant de Component");

		// Ajoute un nouveau composant de type T
		listManager.push_back(manager);
	}


private:

	std::vector<Manager*> listManager;

protected:

	virtual void Save() {}; 
	virtual void Load(std::wstring wFileDirection) {};



};