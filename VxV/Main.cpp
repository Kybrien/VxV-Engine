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
	unsigned char* data = stbi_load(filename, &width, &height, &numComponents, 4);
	if (data == NULL) {
		std::cerr << "Failed to load texture: " << filename << std::endl;
		return 0;
	}

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	// Load the texture using any two methods
	//GLuint Texture = loadBMP_custom("uvtemplate.bmp");
	//GLuint Texture = loadDDS("uvmap.DDS");

	//// Read our .obj file
	//std::vector< glm::vec3 > vertices;
	//std::vector< glm::vec2 > uvs;
	//std::vector< glm::vec3 > normals;
	//
	/*bool res = loadOBJ("miku.obj", vertices, uvs, normals);
	
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
*/

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
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	/*GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
*/

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
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

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

		size_t totalVertexCount = 0;

		for (size_t s = 0; s < shapes.size(); s++) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureIDs[s]);
			glUniform1i(TextureID, 0);

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
				}

				// Draw the face
				glDrawArrays(GL_TRIANGLES, totalVertexCount + f, 3);
			}

			totalVertexCount += mesh.indices.size();
		}
		//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		//glVertexAttribPointer(
		//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		//	3,                  // size
		//	GL_FLOAT,           // type
		//	GL_FALSE,           // normalized?
		//	0,                  // stride
		//	(void*)0            // array buffer offset
		//);
		//// 2nd attribute buffer : colors
		//glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		//glVertexAttribPointer(
		//	1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		//	2,                                // size
		//	GL_FLOAT,                         // type
		//	GL_FALSE,                         // normalized?
		//	0,                                // stride
		//	(void*)0                          // array buffer offset
		//);

		//// 3rd attribute buffer : normals
		//glEnableVertexAttribArray(2);
		//glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		//glVertexAttribPointer(
		//	2,                                // attribute
		//	3,                                // size
		//	GL_FLOAT,                         // type
		//	GL_FALSE,                         // normalized?
		//	0,                                // stride
		//	(void*)0                          // array buffer offset
		//);


		////// Draw the triangle !
		////glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // 12*3 indices starting at 0 -> 12 triangles

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		////TEST // Draw the triangles !
		//glDrawElements(
		//	GL_TRIANGLES,      // mode
		//	indices.size(),    // count
		//	GL_UNSIGNED_SHORT,   // type
		//	(void*)0           // element array buffer offset
		//);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);
	//glDeleteBuffers(1, &normalbuffer);
	//glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);
	

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}	
