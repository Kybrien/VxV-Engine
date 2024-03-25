#pragma once

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
	static Engine* GetInstance()
	{
		if (!instance)
			instance = new Engine();
		return instance;
	}

	Engine() {
		instance = this;
	}


	void Init() { state = Initializing; }

	void Start() {
		state = Starting;


		// init les managers
	}
	void Update() { state = Running; }



};