#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <filesystem>

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

	bool operator==(const Vertex& other) const {
		return position == other.position && normal == other.normal && texCoord == other.texCoord;
	}
};

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2>& p) const {
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);

		// Mainly for demonstration purposes, i.e. works but is overly simple
		// In the real world, use sth. like boost.hash_combine
		return h1 ^ h2;
	}
};

namespace std {
	template<> struct hash<glm::vec3> {
		size_t operator()(glm::vec3 const& vec) const {
			return ((hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1)) >> 1) ^ (hash<float>()(vec.z) << 1);
		}
	};

	template<> struct hash<glm::vec2> {
		size_t operator()(glm::vec2 const& vec) const {
			return hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1);
		}
	};

	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

struct Vec3Hash {
	size_t operator()(const glm::vec3& vec) const {
		return ((hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1)) >> 1) ^ (hash<float>()(vec.z) << 1);
	}
};

struct Vec2Hash {
	size_t operator()(const glm::vec2& vec) const {
		return hash<float>()(vec.x) ^ (hash<float>()(vec.y) << 1);
	}
};

struct VertexHash {
	size_t operator()(const Vertex& vertex) const {
		return ((Vec3Hash()(vertex.position) ^ (Vec3Hash()(vertex.normal) << 1)) >> 1) ^ (Vec2Hash()(vertex.texCoord) << 1);
	}
};

Vertex createVertexFromIndex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index) {
	Vertex vertex;

	// Position
	vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
	vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
	vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

	// Normal
	vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
	vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
	vertex.normal.z = attrib.normals[3 * index.normal_index + 2];

	// Texture Coordinate
	if (index.texcoord_index != -1) {
		vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
		vertex.texCoord.y = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]; // Flip Y coordinate because OBJ's coordinate system is different from OpenGL's
	}
	else {
		vertex.texCoord = glm::vec2(0.0f, 0.0f);
	}

	return vertex;
}

GLuint loadTexture(const char* filename) {
	int width, height, numComponents;
	//stbi_set_flip_vertically_on_load(true);
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

int main() {
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
	glfwSwapInterval(0);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited movement
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	// Reset mouse position for next frame
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);
	glfwSetCursorPos(window, width / 2, height / 2);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.MIKU", "SimpleFragmentShader.VALORANT");
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

	// Projection matrix: 45° Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera:
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,6.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(9, 5, 10), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

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
	// Before indexing
	size_t totalIndices = 0;
	for (const auto& shape : shapes) {
		totalIndices += shape.mesh.indices.size();
	}
	std::cout << "Before indexing - Total vertices: " << totalIndices / 3 << ", Total triangles: " << totalIndices / 3 << std::endl;
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::unordered_map<Vertex, unsigned int> uniqueVertices;

	for (size_t s = 0; s < shapes.size(); s++) {
		tinyobj::mesh_t& mesh = shapes[s].mesh;
		for (size_t f = 0; f < mesh.indices.size(); f++) {
			tinyobj::index_t index = mesh.indices[f];

			Vertex vertex = {};
			vertex.position = {
				attributes.vertices[3 * index.vertex_index + 0],
				attributes.vertices[3 * index.vertex_index + 1],
				attributes.vertices[3 * index.vertex_index + 2]
			};
			vertex.normal = {
				attributes.normals[3 * index.normal_index + 0],
				attributes.normals[3 * index.normal_index + 1],
				attributes.normals[3 * index.normal_index + 2]
			};
			vertex.texCoord = {
				attributes.texcoords[2 * index.texcoord_index + 0],
				attributes.texcoords[2 * index.texcoord_index + 1]
			};

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<unsigned int>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	GLuint indexbuffer;
	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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
	std::cout << "After indexing - Total vertices: " << uniqueVertices.size() << ", Total triangles: " << indices.size() / 3 << std::endl;

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
