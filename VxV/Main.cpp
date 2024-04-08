#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include "object.hpp"

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

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warnings;
	std::string errors;

	tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, "miku.obj", "");

	std::vector<Vertex> vertices;
	std::vector<std::pair<size_t, size_t>> shapeVertexRanges;
	for (int i = 0; i < shapes.size(); i++) {
		tinyobj::shape_t& shape = shapes[i];
		tinyobj::mesh_t& mesh = shape.mesh;
		size_t startIndex = vertices.size();
		for (int j = 0; j < mesh.indices.size(); j++) {
			tinyobj::index_t i = mesh.indices[j];
			glm::vec3 position = {
				attributes.vertices[i.vertex_index * 3],
				attributes.vertices[i.vertex_index * 3 + 1],
				attributes.vertices[i.vertex_index * 3 + 2]
			};
			glm::vec3 normal = {
				attributes.normals[i.normal_index * 3],
				attributes.normals[i.normal_index * 3 + 1],
				attributes.normals[i.normal_index * 3 + 2]
			};
			glm::vec2 texCoord = {
				attributes.texcoords[i.texcoord_index * 2],
				attributes.texcoords[i.texcoord_index * 2 + 1],
			};
			Vertex vert = { position, normal, texCoord };
			vertices.push_back(vert);
		}
		size_t count = vertices.size() - startIndex;  // Count of vertices for this shape
		shapeVertexRanges.push_back(std::make_pair(startIndex, count));  // Store start index and count
	}

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	
	std::vector<GLuint> textureIDs;
	for (const auto& material : materials) {
		if (!material.diffuse_texname.empty()) {
			GLuint texID = loadTexture(material.diffuse_texname.c_str());
			textureIDs.push_back(texID);
		}
		else {
			textureIDs.push_back(0);  // No texture for this material
		}
	}


	//if (textureIDs.size() == 0) {
	//	GLuint texID = loadTexture("image001.png");
	//	textureIDs.push_back(texID);
	//}
	

	// Get a handle for our uniforms
	glUseProgram(programID);
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	GLuint MaterialAmbientColorID = glGetUniformLocation(programID, "MaterialAmbientColor");
	GLuint MaterialDiffuseColorID = glGetUniformLocation(programID, "MaterialDiffuseColor");
	GLuint MaterialSpecularColorID = glGetUniformLocation(programID, "MaterialSpecularColor");

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
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = myRotationMatrix2 * ModelMatrix;
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		
		glBindVertexArray(VertexArrayID);
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// 1st attribute buffer : vertices
		// Bind our texture in Texture Unit 0

		for (const auto& pair : vertexBuffers) {
			GLuint texID = pair.first.first;
			int matID = pair.first.second;
			const std::vector<Vertex>& vertices = pair.second;

			// Bind the texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texID);
			glUniform1i(TextureID, 0);

			// Set the material properties
			tinyobj::material_t& material = materials[matID];
			GLfloat ambient[3] = { material.ambient[0], material.ambient[1], material.ambient[2] };
			GLfloat diffuse[3] = { material.diffuse[0], material.diffuse[1], material.diffuse[2] };
			GLfloat specular[3] = { material.specular[0], material.specular[1], material.specular[2] };
			glUniform3fv(MaterialAmbientColorID, 1, ambient);
			glUniform3fv(MaterialDiffuseColorID, 1, diffuse);
			glUniform3fv(MaterialSpecularColorID, 1, specular);

			// Bind the vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIDs[pair.first]);

			// Draw the vertices
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));

			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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

	// Cleanup VBO and shader
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteVertexArrays(1, &TextureID);
	glDeleteVertexArrays(1, &LightID);
	glDeleteVertexArrays(1, &MaterialAmbientColorID);
	glDeleteVertexArrays(1, &MaterialDiffuseColorID);
	glDeleteVertexArrays(1, &MaterialSpecularColorID);
	glDeleteVertexArrays(1, &MatrixID);
	glDeleteVertexArrays(1, &ViewMatrixID);
	glDeleteVertexArrays(1, &ModelMatrixID);
	glDeleteVertexArrays(1, &indexbuffer);
	glDeleteVertexArrays(1, textureIDs.data());

	for (const auto& pair : vertexBufferIDs) {
		glDeleteBuffers(1, &pair.second);
	}

	

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}	

