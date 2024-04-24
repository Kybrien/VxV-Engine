#include "EngineSetup.hpp"

int main()
{

	Engine* engine = new Engine();
	EngineGUI* gui = new EngineGUI();
	APIopenGL* apiGraphic = new APIopenGL();
	startup(gui,apiGraphic);
	engine->Init();

	Manager* manager = Manager::GetInstance();
	SceneManager* sceneManager = manager->GetManager<SceneManager>();

	//test
	GameObject* go = sceneManager->gameObjectPool.CreateGoFromPool();

	go->AddComponent<Model>();
	go->GetComponent<Model>()->SetModel("miku");


	GameObject* go2 = sceneManager->gameObjectPool.CreateGoFromPool();

	go2->AddComponent<Model>();
	go2->GetComponent<Model>()->SetModel("cube");

	std::vector<GameObject*> goList = Manager::GetInstance()->GetManager<SceneManager>()->GetCurrentScene()->GetAllGameObjects();
	translateModel(*go->GetComponent<Model>()->GetModel(), glm::vec3(10, 0, 0));
	while (engine->GetBootingState() != EngineState::BootingState::Stopped)
	{
		checkCloseWindow(apiGraphic, engine);
		apiDrawLoopSetup(apiGraphic);
		if (engine->GetActiveState() == EngineState::ActiveState::Edition) {

		}
		else if (engine->GetActiveState() == EngineState::ActiveState::RunTime) {
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - engine->getLastTime();
			engine->setLastTime(currentTime);
		}


		for (GameObject* go : goList)
		{
			Model* modelComp = go->GetComponent<Model>();
			if (modelComp != nullptr)
			{
				apiGraphic->drawingModel(modelComp);
			}
		}

		gui->UpdateGui();
		gui->RenderGui();

		apiGraphic->unbindArrays();
		apiGraphic->swapBuffers();
		glfwPollEvents();
	}


	for (GameObject* go : Manager::GetInstance()->GetManager<SceneManager>()->GetCurrentScene()->GetAllGameObjects())
	{
		Model* model = go->GetComponent<Model>();
		if (model != nullptr)
		{
			apiGraphic->cleanupModel(model);
		}
	}
	apiGraphic->terminate();
	return 0;
}
