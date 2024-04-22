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

	/**
	* Define the states of the engine.
	*/
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
	GLuint VertexArrayID;
	GLuint MatrixID, ViewMatrixID, ModelMatrixID;


	Manager* manager;

	static Engine* GetInstance()
	{
		if (!instance)
			instance = new Engine();
		return instance;
	}

	/**  
	* @param none
	*/
	EngineState GetState() {
		return state;
	}

	Engine()
	{
		instance = this;
	}

	/**
	 * Initialize all the managers and the engine.
	 */
	void Init(GLuint _VertexArrayID, GLuint _MatrixID, GLuint _ViewMatrixID, GLuint _ModelMatrixID) {
		state = Initializing; 

		VertexArrayID = _VertexArrayID;
		MatrixID = _MatrixID;
		ViewMatrixID = _ViewMatrixID;
		ModelMatrixID = _ModelMatrixID;

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
