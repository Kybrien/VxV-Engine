#pragma once
#include "Engine.hpp"
#include "Light.h"

Engine* Engine::m_instance = nullptr;

void Engine::InitEngine() {
	m_engineState->StartBooting();


	m_gui = new EngineGUI();
	m_apiGraphic = new APIopenGL();

	Startup(m_gui, m_apiGraphic);
	manager = Manager::GetInstance();
	// init les managers
	manager->Init();
	m_sceneManager = manager->GetManager<SceneManager>();



	m_engineState->ReadyBooting();
	Start();
}

void Engine::Start()
{
	m_engineState->StartRunning();


	m_engineState->Running();
	Update();
}

void Engine::Update()
{
	while (GetBootingState() == EngineState::BootingState::Running)
	{

		CheckCloseWindow(m_apiGraphic, this->GetInstance());
		ApiDrawLoopSetup(m_apiGraphic);
		if (GetActiveState() == EngineState::ActiveState::Edition) {

		}
		else if (GetActiveState() == EngineState::ActiveState::RunTime) {

			PlayUpdate();
		}

		m_gui->UpdateGui();
		m_gui->RenderGui();
		UpdateGameObjectList();

		if (m_goList.size() > 0)
		{
			for (GameObject* go : m_goList)
			{
				Model* modelComp = go->GetComponent<Model>();
				if (modelComp != nullptr)
				{
					m_apiGraphic->drawingModel(go);
				}
			}
		}
		m_apiGraphic->unbindArrays();
		m_apiGraphic->swapBuffers();
		glfwPollEvents();
	}
}


//init go
void Engine::PlayInit()
{
	m_sceneManager->GetCurrentScene()->Init();
}

//start go
void Engine::PlayStart()
{
	m_sceneManager->GetCurrentScene()->Start();
}

//stop go
void Engine::PlayUpdate()
{
	m_sceneManager->GetCurrentScene()->Update();
}

void Engine::Stop(APIopenGL* _apiGraphic)
{
	std::cout << "Engine Stop" << std::endl;
	m_engineState->AskToStop();
	for (GameObject* go : Manager::GetInstance()->GetManager<SceneManager>()->GetCurrentScene()->GetAllGameObjects())
	{
		Model* model = go->GetComponent<Model>();
		if (model != nullptr)
		{
			_apiGraphic->cleanupModel(model);
		}
	}
	m_apiGraphic->terminate();
}

void Engine::Startup(EngineGUI* _gui, APIopenGL* _apiGraphic) {
	_apiGraphic->initialize();
	_apiGraphic->setShaders("SimpleVertexShader.MIKU", "SimpleFragmentShader.VALORANT");
	_apiGraphic->setBackgroundColor(0.0f, 0.0f, 0.4f, 0.0f);
	_apiGraphic->setCamera(45.0f, 4.0f, 3.0f, 0.1f, 100.0f, 9, 5, 1, 0, 0, 0, 0, 1, 0);
	_apiGraphic->setHandles();
	_apiGraphic->setLightColor(glm::vec3(1, 1, 1));
	_apiGraphic->setLightPower(80.0f);

	_gui->initImgui(_apiGraphic->getWindow());

}

void Engine::CheckCloseWindow(APIopenGL* _apiGraphic, Engine* _engine)
{
	if (glfwWindowShouldClose(_apiGraphic->getWindow())) {
		Stop(_apiGraphic);
		_engine->GetEngineState()->Stopped();
	}
}

void Engine::ApiDrawLoopSetup(APIopenGL* _apiGraphic)
{
	_apiGraphic->clearScreen();
	_apiGraphic->useShader();

	for (GameObject* go : m_goList)
	{
		Light* light = go->GetComponent<Light>();
		if (light != nullptr)
		{
			glm::vec3 lightPos = go->GetComponent<Transform>()->GetPosition();
			glUniform3f(_apiGraphic->getLightID(), lightPos.x, lightPos.y, lightPos.z);
		}
	}

	computeMatricesFromInputs(_apiGraphic->getWindow());
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
}