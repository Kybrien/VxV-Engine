#include <stdio.h>
#include <stdlib.h>
#include "EngineGUI.h"
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include "object.hpp"

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
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	std::unordered_map<std::pair<GLuint, int>, std::vector<Vertex>, pair_hash> vertexBuffers;

	for (size_t s = 0; s < shapes.size(); s++) {
		tinyobj::mesh_t& mesh = shapes[s].mesh;
		for (size_t f = 0; f < mesh.indices.size(); f += 3) {
			GLuint texID = textureIDs[mesh.material_ids[f / 3]];
			int matID = mesh.material_ids[f / 3];
			Vertex v1 = createVertexFromIndex(attributes, mesh.indices[f]);
			Vertex v2 = createVertexFromIndex(attributes, mesh.indices[f + 1]);
			Vertex v3 = createVertexFromIndex(attributes, mesh.indices[f + 2]);
			vertexBuffers[std::make_pair(texID, matID)].push_back(v1);
			vertexBuffers[std::make_pair(texID, matID)].push_back(v2);
			vertexBuffers[std::make_pair(texID, matID)].push_back(v3);
		}
	}

	std::unordered_map<std::pair<GLuint, int>, GLuint, pair_hash> vertexBufferIDs;

	for (const auto& pair : vertexBuffers) {
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * pair.second.size(), &pair.second[0], GL_STATIC_DRAW);
		vertexBufferIDs[pair.first] = vertexbuffer;
	}


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

	for (auto& object : objects) {
		cleanup(window, object.vertexbuffer, programID, VertexArrayID);
	}
	return 0;
}
