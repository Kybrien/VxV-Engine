#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "Externes/stb/stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "Externes/tiny_obj_loader.h"
using namespace glm;
#include "loadShader.hpp"
#include "controls.hpp"
#include "texture.hpp"
#include "objloader.hpp"
#include "vboindexer.hpp"

class Vertex {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

GLuint loadTexture(const char* filename) {
	int width, height, numComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename, &width, &height, &numComponents, 4);
	if (data == NULL) {
		std::cerr << "Failed to load texture: " << filename << std::endl;
		return 0;
	}
	std::cout << "Loaded texture: " << filename << " (" << width << "x" << height << ")" << std::endl;
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return texID;
}

void init(GLFWwindow** window) {

	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Prepare the settings for the window
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
	
	// Open a window and create its OpenGL context
	*window = glfwCreateWindow(1280, 720, "VxV", NULL, NULL);
	if (*window == NULL) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Initialize GLEW
	glfwMakeContextCurrent(*window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(EXIT_FAILURE);
	}
}

void setupInput(GLFWwindow* window) {
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Set the mouse at the center of the screen
	glfwPollEvents();
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);
}

glm::mat4 initializeProjectionMatrix() {
	// Projection matrix: 45° Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
	return glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
}

glm::mat4 initializeViewMatrix() {
	// Camera matrix
	return glm::lookAt(glm::vec3(9, 5, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void loadModelAndCreateBuffers(const std::string& filename, std::vector<Vertex>& vertices, std::vector<std::pair<size_t, size_t>>& shapeVertexRanges, GLuint& vertexbuffer) {
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warnings, errors;

	if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, filename.c_str())) {
		std::cerr << "Could not load OBJ file: " << filename << std::endl;
		return;
	}

	for (const auto& shape : shapes) {
		const auto& mesh = shape.mesh;
		size_t startIndex = vertices.size();
		for (const auto& index : mesh.indices) {
			glm::vec3 position = {
				attributes.vertices[3 * index.vertex_index + 0],
				attributes.vertices[3 * index.vertex_index + 1],
				attributes.vertices[3 * index.vertex_index + 2],
			};
			glm::vec3 normal = {
				attributes.normals[3 * index.normal_index + 0],
				attributes.normals[3 * index.normal_index + 1],
				attributes.normals[3 * index.normal_index + 2],
			};
			glm::vec2 texCoord = {
				attributes.texcoords[2 * index.texcoord_index + 0],
				attributes.texcoords[2 * index.texcoord_index + 1],
			};
			vertices.emplace_back(Vertex{ position, normal, texCoord });
		}
		size_t count = vertices.size() - startIndex;
		shapeVertexRanges.emplace_back(startIndex, count);
	}
	/*setupBuffers(vertexbuffer, vertices);*/
}

void setupVertexAttributes() {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);  // Position attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));  // Normal attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));  // Texture coordinate attribute
}

void initOpenGLSettings() {
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
}

void setupMatricesAndUniforms(GLuint programID, GLuint& MatrixID, GLuint& ViewMatrixID, GLuint& ModelMatrixID) {
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
}

void setupBuffers(GLuint& vertexbuffer, const std::vector<Vertex>& vertices) {
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void setupLighting(GLuint programID, GLuint& LightID) {
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
}

void cleanup(GLFWwindow* window, GLuint vertexbuffer, GLuint programID, GLuint VertexArrayID) {
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

int main() {
	//On initialise tout
	GLFWwindow* window;
	init(&window);
	setupInput(window);
	initOpenGLSettings();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.MIKU", "SimpleFragmentShader.VALORANT");

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glm::mat4 Projection = initializeProjectionMatrix();
	// Ortho camera:
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,6.0f,100.0f); // In world coordinates

	glm::mat4 View = initializeViewMatrix();

	GLuint MatrixID, ViewMatrixID, ModelMatrixID;
	setupMatricesAndUniforms(programID, MatrixID, ViewMatrixID, ModelMatrixID);

	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warnings;
	std::string errors;

	tinyobj::LoadObj(&attributes, &shapes, &materials, &warnings, &errors, "911.obj", "");

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
	setupBuffers(vertexbuffer, vertices);
	
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

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	GLuint LightID;
	setupLighting(programID, LightID);
	GLuint MaterialAmbientColorID = glGetUniformLocation(programID, "MaterialAmbientColor");
	GLuint MaterialDiffuseColorID = glGetUniformLocation(programID, "MaterialDiffuseColor");
	GLuint MaterialSpecularColorID = glGetUniformLocation(programID, "MaterialSpecularColor");

	glBindVertexArray(0);
	// glfwGetTime is called only once, the first time this function is called
	double lastTime = glfwGetTime();
	double lastTimeFPS = lastTime;
	int nbFrames = 0;
	//update be  like

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTimeFPS >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTimeFPS += 1.0;
		}
		float deltaTime = float(currentTime - lastTime);
		float angle = deltaTime *25.0f;

		glm::mat4 myRotationMatrix2 = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0));
		
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

		setupVertexAttributes();
		size_t totalVertexCount = 0;
		for (size_t s = 0; s < shapes.size(); s++) {

			tinyobj::mesh_t& mesh = shapes[s].mesh;
			for (size_t f = 0; f < mesh.indices.size(); f += 3) {
				if (mesh.material_ids[f / 3] >= 0) {
					tinyobj::material_t& material = materials[mesh.material_ids[f / 3]];
					GLfloat ambient[3] = { material.ambient[0], material.ambient[1], material.ambient[2] };
					GLfloat diffuse[3] = { material.diffuse[0], material.diffuse[1], material.diffuse[2] };
					GLfloat specular[3] = { material.specular[0], material.specular[1], material.specular[2] };
					glUniform3fv(MaterialAmbientColorID, 1, ambient);
					glUniform3fv(MaterialDiffuseColorID, 1, diffuse);
					glUniform3fv(MaterialSpecularColorID, 1, specular);
					//std::cout << "name: " <<  material.name << std::endl;
					GLuint texID = textureIDs[mesh.material_ids[f / 3]];
					if (texID != 0) {  // Check if a texture exists for this material
						//std::cout << "texture ID: " << texID << std::endl;
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, texID);
						glUniform1i(TextureID, 0);
					}
				}
				// Draw the face
				glDrawArrays(GL_TRIANGLES, totalVertexCount + f, 3);
				// Deactivate the texture
				if (mesh.material_ids[f / 3] >= 0) {
					GLuint texID = textureIDs[mesh.material_ids[f / 3]];
					if (texID != 0) {  // Check if a texture exists for this material
						glBindTexture(GL_TEXTURE_2D, 0);
					}
				}
			}
			totalVertexCount += mesh.indices.size();
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	cleanup(window, vertexbuffer, programID, VertexArrayID);
	return 0;
}	
