#pragma once

#include "SceneManager.h"

class Engine {
private:


	enum EngineState {
		Off = 0,
		Initializing,
		Ready,
		Starting,
		Running,
		AsktoStop,
		Stopping,
		Stopped
	};


	static Engine* instance;
	EngineState state = Off;





public:
	Manager* manager;

	static Engine* GetInstance()
	{
		if (!instance)
			instance = new Engine();
		return instance;
	}

	EngineState GetState() {
		return state;
	}

	Engine() {
		instance = this;
	}


	void Init() { 
		state = Initializing; 

		state = Ready;
		Start();
	}

	void Start() {
		state = Starting;


		manager = Manager::GetInstance();
		manager->Init();
		// init les managers

		Update();
	}
	void Update() { state = Running; }

};