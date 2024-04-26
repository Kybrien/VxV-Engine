#pragma once
#include <string>
#include <vector>

#include "Manager.h"
#include "Scene.h"
#include "MemoryPool.h"

class SceneManager : public Manager
{
private:
	/**
	* @brief Vector containing pointers to Scene objects.
	*/
	std::vector<Scene*> scenes;

	/**
	* @brief Pointer to the current Scene object.
	* Represents the currently active scene.
	*/
	Scene* currentScene;

	/**
	* @brief Static string representing the file direction.
	* Used to store the directory where files are located.
	*/
	static std::string fileDirection;

	/**
	* @brief Static string representing the file extension.
	* Used to specify the file extension for files.
	*/
	static std::string extention;


public:
	/**
	* @brief Memory pool for GameObject instances.
	* Uses MemPool_Linear as the underlying memory management strategy.
	*/
	MemoryPool<GameObject, MemPool_Linear<GameObject>> gameObjectPool;

	/**
	* @brief Constructs a SceneManager object.
	* @param manager Pointer to the Manager object associated with the SceneManager.
	*/
	SceneManager(Manager* manager);

	/**
	* @brief Adds a scene to the SceneManager.
	* @param scene Pointer to the Scene object to be added.
	* @return void
	*/
	void AddScene(Scene* scene)
	{
		scenes.push_back(scene);
	}

	/**
	* @brief Retrieves the list of scenes managed by the SceneManager.
	* @return A vector containing pointers to Scene objects.
	*/
	std::vector<Scene*> GetScenes()
	{
		return scenes;
	}


	void Save() override;
	void Load(std::wstring wFileDirection, std::wstring wFileName) override;

	/**
	* @brief Retrieves the current scene.
	* @return A pointer to the current Scene object.
	*/
	Scene* GetCurrentScene()
	{
		return currentScene;
	}
};
