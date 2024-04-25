#include "EngineSetup.hpp"


const float DESIRED_FPS = 60.0f;
const float FRAME_TIME = 1.0f / DESIRED_FPS;

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
	go2->GetComponent<Model>()->SetModel("miku");

	go->AddChild(go2);

	std::vector<GameObject*> goList = Manager::GetInstance()->GetManager<SceneManager>()->GetCurrentScene()->GetAllGameObjects();
	go2->GetComponent<Transform>()->Translate(glm::vec3(10, 0, 0));
	//*test

	while (engine->GetBootingState() != EngineState::BootingState::Stopped)
	{
		checkCloseWindow(apiGraphic, engine);
		apiDrawLoopSetup(apiGraphic);
		if (engine->GetActiveState() == EngineState::ActiveState::Edition) {

		}
		else if (engine->GetActiveState() == EngineState::ActiveState::RunTime) {
			float currentTime = glfwGetTime();
			float deltaTime = currentTime - engine->getLastTime();
			
			if (deltaTime > FRAME_TIME) {
				engine->Update();
				std::cout << "Update" << std::endl;
			}
			engine->setLastTime(currentTime);
		}

		//TODO: Move it to EngineSetup
		for (GameObject* go : goList)
		{
			Model* modelComp = go->GetComponent<Model>();
			if (modelComp != nullptr)
			{
				apiGraphic->drawingModel(go);
			}
		}

		gui->UpdateGui();
		gui->RenderGui();

		apiGraphic->unbindArrays();
		apiGraphic->swapBuffers();
		glfwPollEvents();
	}

	//TODO : Move it to EngineSetup
	//engine->Stop(apiGraphic);

	apiGraphic->terminate();
	return 0;
}
