#pragma once

#include "APIopenGL.hpp"
#include "ModelManager.h"
#include "PrefabManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "EngineState.h"


/**
 * Define the state of the engine.
 */
class Engine {
private:
	static Engine* instance;
	SceneManager* sceneManager;	
	EngineState* engineState = EngineState::GetInstance();
	float lastTime = glfwGetTime();
public:
	Manager* manager;

	static Engine* GetInstance()
	{
		if (!instance)
			instance = new Engine();
		return instance;
	}


	Engine() {
		
	}

	/**
	 * Initialize all the managers and the engine.
	 */
	void Init() {
		engineState->StartBooting();

		manager = Manager::GetInstance();
		manager->Init();
		sceneManager = manager->GetManager<SceneManager>();
		// init les managers
		engineState->ReadyBooting();

		//init go
		sceneManager->GetCurrentScene()->Init();
		Start();
	}

	void Start()
	{
		engineState->StartRunning();


		// start go
		sceneManager->GetCurrentScene()->Start();

		engineState->Running();
		Update();
	}

	void Update()
	{
		while (engineState->GetActiveState() == EngineState::ActiveState::RunTime) {

		sceneManager->GetCurrentScene()->Update();
		}
	}

	void Stop(APIopenGL* _apiGraphic)
	{
		std::cout << "Engine Stop" << std::endl;
		engineState->AskToStop();
		for (GameObject* go : Manager::GetInstance()->GetManager<SceneManager>()->GetCurrentScene()->GetAllGameObjects())
		{
			Model* model = go->GetComponent<Model>();
			if (model != nullptr)
			{
				_apiGraphic->cleanupModel(model);
			}
		}
	}



	EngineState* GetEngineState()
	{
		return engineState;
	}

	EngineState::ActiveState GetActiveState()
	{
		return engineState->GetActiveState();
	}

	EngineState::BootingState GetBootingState()
	{
		return engineState->GetBootingState();
	}

	double getLastTime()
	{
		return lastTime;
	}
	void setLastTime(double _lastTime)
	{
		lastTime = _lastTime;
	}
};
