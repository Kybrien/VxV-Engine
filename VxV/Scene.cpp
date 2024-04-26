#include "Scene.h"
#include "SceneManager.h"

Scene::Scene(std::string name_) {
	Manager* manager = Manager::GetInstance();
	sceneManager = manager->GetManager<SceneManager>(); 

	name = name_;
	origin = glm::vec3(0, 0, 0);

	sceneManager->AddScene(this); 
}

void Scene::RemoveGameObject(int id)
{
	Manager* manager = Manager::GetInstance();
	SceneManager* sceneManager = manager->GetManager<SceneManager>();
	for (int i = 0; i < allGameObjects.size(); i++)
	{
		if (allGameObjects[i]->GetId() == id) {
			sceneManager->gameObjectPool.Free(allGameObjects[i]);
			sceneManager->gameObjectPool.Free(gameObjects[i]);
			allGameObjects.erase(allGameObjects.begin() + i);
			gameObjects.erase(gameObjects.begin() + i);
			break;
		}
	}
}