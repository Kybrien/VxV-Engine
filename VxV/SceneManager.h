#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Manager.h"
#include "Scene.h"

class SceneManager : public Manager {
private:
	std::vector<Scene*> scenes;

	Scene* currentScene;


public:

	SceneManager(Manager* manager);

	

	void AddScene(Scene* scene) {
		scenes.push_back(scene);
	}

	std::vector<Scene*> GetScenes() {
		return scenes;
	}

	void Save() override;
	void Load(std::wstring wFileDirection) override;

	Scene* GetCurrentScene() {
		return currentScene;
	}
};