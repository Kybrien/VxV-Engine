#pragma once
#include <iostream>
#include <list>
#include <string>

#include "Scene.h"

class SceneManager {
private:
	std::list<Scene*> scenes;
	static SceneManager* instance;

	Scene* currentScene;


public:

	SceneManager();

	static SceneManager* GetInstance()
	{
		if (!instance)
			instance = new SceneManager();
		return instance;
	}

	void AddScene(Scene* scene) {
		scenes.push_back(scene);
	}

	std::list<Scene*> GetScenes() {
		return scenes;
	}
};