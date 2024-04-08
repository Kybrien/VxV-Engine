#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include "MemoryPool.h"







#include "object.hpp"
#include "EngineGUI.h"

int main() {
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
	// Chargez la police qui supporte les caract�res accentu�s
	ImFont* font = io.Fonts->AddFontFromFileTTF("monocraft.ttf", 17);

	// V�rifiez si la police a �t� charg�e correctement
	if (font == nullptr)
	{
		std::cerr << "Erreur lors du chargement de la police." << std::endl;
	}
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	do {
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

	cleanup(window, objects, programID, VertexArrayID, TextureID, LightID,
		MaterialAmbientColorID, MaterialDiffuseColorID, MaterialSpecularColorID, MatrixID, ViewMatrixID, ModelMatrixID);
	return 0;
}
