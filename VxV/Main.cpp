#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include "loadShader.hpp"








#include "Debug.h"

#include "Engine.h"


using namespace glm;

int main() {
	

	// ---------------------------------- Autres Tests ------------------------- //
	Engine* engine = new Engine();
	engine->Init();
	
	
	std::cout << engine->GetState() << std::endl;

	SceneManager* sc = engine->manager->GetManager<SceneManager>();

	Scene* currentScene = sc->GetCurrentScene();

	if (currentScene->GetAllGameObjects().empty()) {

		GameObject* cube = new GameObject("cube", false);
		cube->GetComponent<Transform>()->position.x = 10;

		Prefab* prefabCube = new Prefab(cube, "PrefabCube");
		GameObject* cercle = new GameObject("", false);


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

		Debug::Log(go->name + " : " + go->GetComponent<ScriptingComponent>()->name + "\n");
		}
	}

	for (Prefab* prefab : engine->manager->GetManager<PrefabManager>()->GetPrefabs()) {
		if (prefab->getGameObject().GetComponent<ScriptingComponent>() != nullptr) {


			Debug::Log(prefab->name + " (Prefab) : " + prefab->getGameObject().GetComponent<ScriptingComponent>()->name + "\n");
		}
	}

	engine->manager->GetManager<PrefabManager>()->Save();
	
	
	
	
	
	
	
	
	// ---------------------------------- Fenetre ------------------------- //
	
	
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

	// Open a window and create its OpenGL context
	GLFWwindow* window;
	window = glfwCreateWindow(1280, 720, "VxV", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);// Initialize GLEW

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat g_vertex_buffer_data[] = {
	   -0.2f, -1.0f, 0.0f,
	   0.0f, 1.0f, 0.0f,
	   0.2f, -1.0f, 0.0f,
	};
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);



	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.MIKU", "SimpleFragmentShader.VALORANT");
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		// Draw triangle...
		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);






	


	return 0;
}	

