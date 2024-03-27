#include "Scene.h"
#include "SceneManager.h"

Scene::Scene(std::string name_) {
	Manager* manager = Manager::GetInstance();
	sceneManager = manager->GetManager<SceneManager>(); 

	name = name_;
	origin = glm::vec3(0, 0, 0);

	sceneManager->AddScene(this); 
}