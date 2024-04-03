#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Manager.h"
#include "Scene.h"
#include "MemoryPool.h"

class SceneManager : public Manager {
private:
	std::vector<Scene*> scenes;
	Scene* currentScene;

	static std::string fileDirection;
	static std::string extention;

public:
	MemoryPool<GameObject, MemPool_Linear<GameObject>> m_gameObjectPool;
	SceneManager(Manager* manager);

	

	void AddScene(Scene* scene) {
		scenes.push_back(scene);
	}

	std::vector<Scene*> GetScenes() {
		return scenes;
	}


	void Save() override;
	void Load(std::wstring wFileDirection, std::wstring wFileName) override;

	Scene* GetCurrentScene() {
		return currentScene;
	}
};