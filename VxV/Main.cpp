#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include "object.hpp"
#include "EngineGUI.h"
#include "Debug.h"
#include "Engine.h"

int main() {
		EngineGUI gui;

	// ---------------------------------- Autres Tests ------------------------- //
	Engine* engine = new Engine();
	engine->Init();
	
	
	std::cout << engine->GetState() << std::endl;

	SceneManager* sc = engine->manager->GetManager<SceneManager>();

	Scene* currentScene = sc->GetCurrentScene();

	if (currentScene->GetAllGameObjects().empty()) {

		GameObject* cube = new GameObject("cube");
		cube->GetComponent<Transform>()->position.x = 10;

		Prefab* prefabCube = new Prefab(cube, "PrefabCube");
		GameObject* cercle = new GameObject("cercle", false, prefabCube);
		cercle->GetComponent<Transform>()->position.x = 1000;

		Script* escript = ScriptManager::NewScript("CercleScript");
		cercle->AddComponent<ScriptingComponent>();
		cercle->GetComponent<ScriptingComponent>()->AddScript(escript);




		GameObject* child = new GameObject("child", false);
		child->GetComponent<Transform>()->position = glm::vec3(10, 0, 0);

		cube->AddChild(child);
		child->AddChild(cercle);

		sc->Save();
	}

	
	for (GameObject* go : currentScene->GetAllGameObjects()) {
		if (go->GetComponent<ScriptingComponent>() != nullptr) {

		Debug::Log(go->name + " : " + go->GetComponent<ScriptingComponent>()->GetScript()->name + "\n");
		}
	}

	for (Prefab* prefab : engine->manager->GetManager<PrefabManager>()->GetPrefabs()) {
		if (prefab->getGameObject()->GetComponent<ScriptingComponent>() != nullptr) {


			Debug::Log(prefab->name + " (Prefab) : " + prefab->getGameObject()->GetComponent<ScriptingComponent>()->GetScript()->name + "\n");
		}
	}

	engine->manager->GetManager<PrefabManager>()->Save();
	
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
	glm::mat4 Projection = initializeProjectionMatrix();
	glm::mat4 View = initializeViewMatrix();

	std::vector<Object> objects;
	addNewObject("miku.obj", objects);

	// Get a handle for our uniforms
	GLuint TextureID, LightID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID;
	setupHandlesForUniforms(programID, TextureID, LightID, MaterialAmbientColorID,
		MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID);

	glBindVertexArray(0);
	// glfwGetTime is called only once, the first time this function is called
	double lastTime = glfwGetTime();
	double lastTimeFPS = lastTime;
	int nbFrames = 0;
	//update be  like

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Use our shader
		glUseProgram(programID);

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTimeFPS >= 1.0) { // If last printf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame, FPS: %d\n", 1000.0 / double(nbFrames), nbFrames);
			nbFrames = 0;
			lastTimeFPS += 1.0;
		}

		float deltaTime = float(currentTime - lastTime);

		float angle = deltaTime * 50.0f;  // Rotate by 60 degrees
		glm::vec3 axis(0.0f, 0.0f, -1.0f);  // Rotate around the z-axis

		glm::vec3 lightPos = glm::vec3(0, 0, 8);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(window);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		for (auto& object : objects) {
			sendMVPData(object, angle, axis, VertexArrayID, MatrixID, ModelMatrixID, ViewMatrixID);
			drawObjects(objects, TextureID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID);
		}


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);

		gui.UpdateGui();
		gui.RenderGui();


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	cleanup(window, objects, programID, VertexArrayID, TextureID, LightID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID);
	return 0;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}	

