#include "loadingShader.hpp"

#include <iostream>
#include "Engine.h"
#include "ModelComponent.hpp"
#include "EngineGUI.h"
#include "WindowGui.h"
#include "Engine.h"
#include "imfilebrowser.h"
#include "Debug.h"

int main() {
	Engine* state = Engine::GetInstance();
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
	glm::mat4 Projection = initializeProjectionMatrix();
	glm::mat4 View = initializeViewMatrix();

	// Get a handle for our uniforms
	GLuint TextureID, LightID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID;
	setupHandlesForUniforms(programID, TextureID, LightID, MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID);

	glBindVertexArray(0);
	// glfwGetTime is called only once, the first time this function is called
	double lastTime = glfwGetTime();
	double lastTimeFPS = lastTime;
	int nbFrames = 0;
	//update be  like

	gui.initImgui(window);

	Engine* engine = new Engine();
	engine->Init();

	Manager* manager = Manager::GetInstance();
	SceneManager* sceneManager = manager->GetManager<SceneManager>();

	GameObject* go = sceneManager->gameObjectPool.CreateGameObject();

	go->AddComponent<Model>();
	go->GetComponent<Model>()->SetModel("miku");


	GameObject* go2 = sceneManager->gameObjectPool.CreateGameObject();

	go2->AddComponent<Model>();
	go2->GetComponent<Model>()->SetModel("cube");

	std::vector<GameObject*> goList = Manager::GetInstance()->GetManager<SceneManager>()->GetCurrentScene()->GetAllGameObjects();
	translateModel(*go->GetComponent<Model>()->GetModel(), glm::vec3(10, 0, 0));
	do
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		

		go->GetComponent<Transform>()->Rotate(180, 180 ,0);
		go->GetComponent<Transform>()->SetRotation(90, 0, 0);

		go->GetComponent<Transform>()->SetPosition(glm::vec3(0, 0, 0));

		go->GetComponent<Transform>()->SetScale(glm::vec3(10, 1, 1));



		glm::vec3 lightPos = glm::vec3(0, 0, 8);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(window);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		for (GameObject* go : goList)
		{
			Model* modelComp = go->GetComponent<Model>();
			if (modelComp != nullptr)
			{
				sendMVPData(*(go->GetComponent<Model>()->GetModel()), VertexArrayID, MatrixID, ModelMatrixID, ViewMatrixID);
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
	} while (state->GetState() == Engine::EngineState::Running);

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
	return 0;
}
