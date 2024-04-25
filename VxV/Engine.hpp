#pragma once

#include "APIopenGL.hpp"
#include "ModelManager.h"
#include "PrefabManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "EngineState.h"
#include "EngineGUI.h"
#include <iostream>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imfilebrowser.h"

#include "Debug.h"


/**
 * Define the state of the engine.
 */
class Engine {
private:
	static Engine* instance;
	SceneManager* sceneManager;
	EngineState* engineState = EngineState::GetInstance();
	float lastTime = glfwGetTime();
	std::vector<GameObject*> goList;
	EngineGUI* gui;
	APIopenGL* apiGraphic;
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
	void InitEngine();

	void Start();

	void Update();

	void Stop(APIopenGL* _apiGraphic);

	void startup(EngineGUI* _gui, APIopenGL* _apiGraphic);
	void apiDrawLoopSetup(APIopenGL* _apiGraphic);
	void checkCloseWindow(APIopenGL* _apiGraphic, Engine* _engine);

	void PlayInit();
	void PlayStart();
	void PlayUpdate();
	void PlayStop();

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
