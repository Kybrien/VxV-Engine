#include "EngineSetup.hpp"

void startup(EngineGUI* _gui, APIopenGL* _apiGraphic) {
	_apiGraphic->initialize();
	_apiGraphic->setShaders("SimpleVertexShader.MIKU", "SimpleFragmentShader.VALORANT");
	_apiGraphic->setBackgroundColor(0.0f, 0.0f, 0.4f, 0.0f);
	_apiGraphic->setCamera(45.0f, 4.0f, 3.0f, 0.1f, 100.0f, 9, 5, 1, 0, 0, 0, 0, 1, 0);
	_apiGraphic->setHandles();
	_apiGraphic->setLightColor(glm::vec3(1, 1, 1));
	_apiGraphic->setLightPower(50.0f);

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

void temp() {
	EngineGUI gui;
	//On initialise tout
	GLFWwindow* window;
	init(window);
	setupInput(window);
	initOpenGLSettings();
	GLuint VertexArrayID;
	initializeVertexArrayObject(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.MIKU", "SimpleFragmentShader.VALORANT");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Ortho camera:
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,6.0f,100.0f); // In world coordinates
	glm::mat4 Projection = initializeProjectionMatrix(45.0f, 4.0f, 3.0f, 0.1f, 100.0f);
	glm::mat4 View = initializeViewMatrix(9, 5, 1, 0, 0, 0, 0, 1, 0);

	// Get a handle for our uniforms
	GLuint TextureID, LightID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID;
	setupHandlesForUniforms(programID, TextureID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID);

	glBindVertexArray(0);
	// glfwGetTime is called only once, the first time this function is called
	double lastTime = glfwGetTime();
	double lastTimeFPS = lastTime;
	int nbFrames = 0;
	//update be  like

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// Chargez la police qui supporte les caractères accentués
	ImFont* font = io.Fonts->AddFontFromFileTTF("monocraft.ttf", 17);

	// V�rifiez si la police a �t� charg�e correctement
	if (font == nullptr)
	{
		std::cerr << "Erreur lors du chargement de la police." << std::endl;
	}
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Engine* engine = new Engine();
	engine->Init();

	Manager* manager = Manager::GetInstance();
	SceneManager* sceneManager = manager->GetManager<SceneManager>();

	GameObject* go = sceneManager->gameObjectPool.CreateGoFromPool();

	go->AddComponent<Model>();
	go->GetComponent<Model>()->SetModel("miku");


	GameObject* go2 = sceneManager->gameObjectPool.CreateGoFromPool();

	go2->AddComponent<Model>();
	go2->GetComponent<Model>()->SetModel("cube");

	std::vector<GameObject*> goList = Manager::GetInstance()->GetManager<SceneManager>()->GetCurrentScene()->GetAllGameObjects();
	translateModel(*go->GetComponent<Model>()->GetModel(), glm::vec3(10, 0, 0));
	do
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Use our shader
		glUseProgram(programID);

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTimeFPS >= 1.0)
		{
			// If last printf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame, FPS: %d\n", 1000.0 / double(nbFrames), nbFrames);
			nbFrames = 0;
			lastTimeFPS += 1.0;
		}



		go->GetComponent<Transform>()->Rotate(180, 180, 0);
		go->GetComponent<Transform>()->SetRotation(90, 0, 0);

		go->GetComponent<Transform>()->SetPosition(glm::vec3(0, 0, 0));

		go->GetComponent<Transform>()->SetScale(glm::vec3(10, 1, 1));



		glm::vec3 lightPos = glm::vec3(0, 0, 8);
		//glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(window);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		for (GameObject* go : goList)
		{
			Model* modelComp = go->GetComponent<Model>();
			if (modelComp != nullptr)
			{
				sendMVPData(*(modelComp->GetModel()), VertexArrayID, MatrixID, ModelMatrixID, ViewMatrixID);
				drawModel(modelComp->GetModel(), TextureID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID);
			}
		}

		gui.UpdateGui();

		gui.RenderGui();

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	for (GameObject* go : Manager::GetInstance()->GetManager<SceneManager>()->GetCurrentScene()->GetAllGameObjects())
	{
		Model* modelComp = go->GetComponent<Model>();
		if (modelComp != nullptr)
		{
			auto& object = *(modelComp->GetModel());

			cleanup(window, object);
		}
	}
	finishProgram(programID, VertexArrayID, TextureID, LightID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID);
}