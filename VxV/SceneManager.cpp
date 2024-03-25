#include "SceneManager.h"
#include "json.h"



SceneManager* SceneManager::instance = nullptr;


SceneManager::SceneManager() {
	instance = this;


	//Rechercher les scenes
	//Les foutre dans la liste
	// 


	// Ajouter une scene s'il n'en existe pas déjà
	if (scenes.empty()) {
		new Scene;
	}

	currentScene = scenes.front();

	for (Scene* sc : scenes) {
		std::string str = sc->name;
		std::cout << str << std::endl;
	}

}

void SceneManager::SaveScene() {

}