#pragma once

#include "ModelManager.h"
#include "PrefabManager.h"
#include "InputManager.h"
#include "SceneManager.h"


/**
 * Define the state of the engine.
 */
class Engine {
private:
	static Engine* instance;

public:
	enum EngineState {
		Edition = 0,
		Initializing,
		Ready,
		Starting,
		Running,
		AsktoStop,
		Stopping,
		Stopped
	};
private:
	EngineState state = Edition;
public:
	Manager* manager;

	static Engine* GetInstance()
	{
		if (!instance)
			instance = new Engine();
		return instance;
	}

	EngineState const GetState() {
		return state;
	}

	Engine() {
		instance = new Engine();
	}

	/**
	 * Initialize all the managers and the engine.
	 */
	void Init() { 
		state = Initializing; 

		manager = Manager::GetInstance();
		manager->Init();
		// init les managers
		state = Ready;

		//init go
		Start();
	}

	void Start()
	{
		state = Starting;
		// start go

		Update();
	}

	void Update()
	{
		state = Running;
	}
};
