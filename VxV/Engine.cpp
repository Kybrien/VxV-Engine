#pragma once
#include "Engine.hpp"

Engine* Engine::instance = nullptr;

void Engine::InitEngine() {
	engineState->StartBooting();


	gui = new EngineGUI();
	apiGraphic = new APIopenGL();

	startup(gui, apiGraphic);
	manager = Manager::GetInstance();
	// init les managers
	manager->Init();
	sceneManager = manager->GetManager<SceneManager>();
	goList = sceneManager->GetCurrentScene()->GetAllGameObjects();



	engineState->ReadyBooting();
	Start();
}

void Engine::Start()
{
	engineState->StartRunning();


	engineState->Running();
	Update();
}

void Engine::Update()
{
	while (GetBootingState() == EngineState::BootingState::Running)
	{
		checkCloseWindow(apiGraphic, this->GetInstance());
		apiDrawLoopSetup(apiGraphic);
		if (GetActiveState() == EngineState::ActiveState::Edition) {

		}
		else if (GetActiveState() == EngineState::ActiveState::RunTime) {
			PlayUpdate();
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
}


//init go
void Engine::PlayInit()
{
	sceneManager->GetCurrentScene()->Init();
}

//start go
void Engine::PlayStart()
{
	sceneManager->GetCurrentScene()->Start();
}

//stop go
void Engine::PlayUpdate()
{

	sceneManager->GetCurrentScene()->Update();
}

void Engine::Stop(APIopenGL* _apiGraphic)
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
	apiGraphic->terminate();
}

void Engine::startup(EngineGUI* _gui, APIopenGL* _apiGraphic) {
	_apiGraphic->initialize();
	_apiGraphic->setShaders("SimpleVertexShader.MIKU", "SimpleFragmentShader.VALORANT");
	_apiGraphic->setBackgroundColor(0.0f, 0.0f, 0.4f, 0.0f);
	_apiGraphic->setCamera(45.0f, 4.0f, 3.0f, 0.1f, 100.0f, 9, 5, 1, 0, 0, 0, 0, 1, 0);
	_apiGraphic->setHandles();
	_apiGraphic->setLightColor(glm::vec3(1, 1, 1));
	_apiGraphic->setLightPower(80.0f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// Load the font
	ImFont* font = io.Fonts->AddFontFromFileTTF("monocraft.ttf", 17);

	// Check if the font has been loaded correctly
	if (font == nullptr)
	{
		std::cerr << "Erreur lors du chargement de la police." << std::endl;
	}
	ImGui_ImplGlfw_InitForOpenGL(_apiGraphic->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void Engine::checkCloseWindow(APIopenGL* _apiGraphic, Engine* _engine)
{
	if (glfwGetKey(_apiGraphic->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(_apiGraphic->getWindow())) {
		_engine->GetEngineState()->Stopped();
	}
}

void Engine::apiDrawLoopSetup(APIopenGL* _apiGraphic)
{
	_apiGraphic->clearScreen();
	_apiGraphic->useShader();
	glm::vec3 lightPos = glm::vec3(0, 0, 8);
	glUniform3f(_apiGraphic->getLightID(), lightPos.x, lightPos.y, lightPos.z);
	computeMatricesFromInputs(_apiGraphic->getWindow());
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
}