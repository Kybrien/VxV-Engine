#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "Scene.h"
#include <fstream>

class SceneManager {
private:
	std::vector<Scene*> scenes;
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

	std::vector<Scene*> GetScenes() {
		return scenes;
	}

	void SaveScene();
	void LoadScene(std::string fileDirection);

	Scene* GetCurrentScene() {
		return currentScene;
	}
};