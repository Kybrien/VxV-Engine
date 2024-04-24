#pragma once

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
	double lastTime = glfwGetTime();
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
		sceneManager->GetCurrentScene()->Update();
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
